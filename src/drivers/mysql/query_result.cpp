#include "query_result.hpp"

#include <sstream>
#include <xdb/types.hpp>

using namespace std ;

namespace xdb {

MySQLQueryResultHandle::~MySQLQueryResultHandle() {
    if (meta_res_) {
        mysql_free_result(meta_res_);
    }
       
    mysql_stmt_free_result(stmt_->handle());
}

MySQLQueryResultHandle::MySQLQueryResultHandle(const std::shared_ptr<MySQLStatementHandle> &stmt): stmt_(stmt) {
    meta_res_ = mysql_stmt_result_metadata(stmt_->handle());
    if ( !meta_res_ ) {
        // No metadata means the query was not a SELECT statement (or returned no fields)
        column_count_ = 0;
        return;
    }

    column_count_ = mysql_num_fields(meta_res_);
    columns_.resize(column_count_);
    output_binds_.resize(column_count_);
    std::memset(output_binds_.data(), 0, output_binds_.size() * sizeof(MYSQL_BIND));

    MYSQL_FIELD* fields = mysql_fetch_fields(meta_res_);

    for (int i = 0; i < column_count_; ++i) {
        columns_[i].type_ = fields[i].type;
        columns_[i].is_null_ = false;
        columns_[i].actual_length_ = 0;
        columns_[i].name_ = std::string(fields[i].name, fields[i].length) ;

        name_to_index_[fields[i].name] = i;

        // Define buffer sizes dynamically based on field types
        unsigned long buffer_size = 0;
        switch (fields[i].type) {
            case MYSQL_TYPE_TINY:      buffer_size = 1; break;
            case MYSQL_TYPE_LONG:      buffer_size = 4; break;
            case MYSQL_TYPE_LONGLONG:  buffer_size = 8; break;
            case MYSQL_TYPE_FLOAT:     buffer_size = 4; break;
            case MYSQL_TYPE_DOUBLE:    buffer_size = 8; break;
            case MYSQL_TYPE_BLOB: 
    // Pre-allocate a 64KB local landing window buffer for performance.
    // If the actual blob is smaller, actual_length tells us its real size.
    // If it's larger, we fetch the remainder dynamically later.
                buffer_size = 65536; break ;
                // For strings, blobs, or dates, allocate max defined length (fallback to 4KB if unset)
            default: 
                buffer_size = fields[i].length > 0 ? fields[i].length + 1 : 4096; 
                break;
        }

        columns_[i].buffer_.resize(buffer_size);

            // Populate the MYSQL_BIND structure for this output column
        output_binds_[i].buffer_type = columns_[i].type_;
        output_binds_[i].buffer = columns_[i].buffer_.data();
        output_binds_[i].buffer_length = buffer_size;
        output_binds_[i].length = &columns_[i].actual_length_;
        output_binds_[i].is_null = &columns_[i].is_null_;
    }

    // Pass the output binding array straight to the statement engine
    if ( mysql_stmt_bind_result(stmt_->handle(), output_binds_.data()) != 0) {
        throw std::runtime_error("Failed to bind output results: " + std::string(mysql_stmt_error(stmt_->handle())));
    }
  
}

void MySQLQueryResultHandle::reset() {
   if (column_count_ > 0 && stmt_) {
        // Seek back to row index 0 (the absolute beginning)
        mysql_stmt_data_seek(stmt_->handle(), 0);
        is_valid_ = false ;
    }
}

void MySQLQueryResultHandle::check_has_row() const {
   if ( !is_valid_ ) throw Exception("Out of bounds: past the end of query results or empty results") ;
}

void MySQLQueryResultHandle::check_idx(int idx) const {
    check_has_row() ;
    if ( idx >= column_count_ ) throw Exception("Invalid column index");
}

bool MySQLQueryResultHandle::next() {
    if ( column_count_ == 0 ) {
        is_valid_ = false ;
        return false;
    }

    int rc = mysql_stmt_fetch(stmt_->handle());
    if ( rc == 1 ) throw Exception(mysql_stmt_error(stmt_->handle()));
    if ( rc == 0 ) { is_valid_ = true ; return true; }            // Row successfully loaded into buffers
    if ( rc == MYSQL_NO_DATA) { is_valid_ = false ; return false; } // End of result set reached
        
    // Handle truncated data or fetch exceptions safely
    if (rc == MYSQL_DATA_TRUNCATED) {
        is_valid_ = true ;
            // Columns were too small for server size; for a basic wrapper we can continue
        return true;
    }

    is_valid_ = false ;
    throw std::runtime_error("Error fetching next statement row: " + std::string(mysql_stmt_error(stmt_->handle())));
    return false ;
}

int MySQLQueryResultHandle::columns() const  {
   return column_count_ ;
}

int MySQLQueryResultHandle::columnType(int idx) const {
    check_idx(idx) ;
    return columns_[idx].type_ ;
}

std::string MySQLQueryResultHandle::columnName(int idx) const  {
    check_idx(idx);
    return columns_[idx].name_ ;
}

int MySQLQueryResultHandle::columnIndex(const std::string &name) const {
    check_has_row() ;
    auto it = name_to_index_.find(name);
    if (it == name_to_index_.end()) {
        throw Exception("Column name '" + name + "' not found in query results.");
    }
    return it->second;
}

bool MySQLQueryResultHandle::columnIsNull(int idx) const {
    check_idx(idx);
    return columns_[idx].is_null_;
}

void MySQLQueryResultHandle::read(int idx, int &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    else std::memcpy(&val, columns_[idx].buffer_.data(), sizeof(int));
}

template<class T>
void read_int(int col_type, const char *data_ptr, int col_actual_length, T &v) {

    switch (col_type) {
        case MYSQL_TYPE_TINY: { // 1 byte (char / tinyint)
            int8_t val = 0;
            std::memcpy(&val, data_ptr, 1);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_SHORT: { // 2 bytes (short / smallint)
            int16_t val = 0;
            std::memcpy(&val, data_ptr, 2);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_LONG: { // 4 bytes (int / mediumint)
            int32_t val = 0;
            std::memcpy(&val, data_ptr, 4);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_LONGLONG: { // 8 bytes (long long / bigint)
            int64_t val = 0;
            std::memcpy(&val, data_ptr, 8);
            v = static_cast<T>(val); // Note: Truncation could happen if bigint > INT_MAX
            break ;
        }
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING: {
            // Convienence fallback: string to int translation "123" -> 123
            std::string str(data_ptr, col_actual_length);
            v = str.empty() ? 0 : std::stoi(str);
            break ;
        }
        default:
            throw std::runtime_error("Type Mismatch: Cannot implicitly convert column type to integer.");
    }
}

void MySQLQueryResultHandle::read(int idx, unsigned int &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
}

void MySQLQueryResultHandle::read(int idx, short int &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
   
}

void MySQLQueryResultHandle::read(int idx, unsigned short &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
}

void MySQLQueryResultHandle::read(int idx, long int &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
}

void MySQLQueryResultHandle::read(int idx, unsigned long &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
   
}

void MySQLQueryResultHandle::read(int idx, bool &v) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) v = false;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, v) ;
}

template<class T>
void read_float(int col_type, const char *data_ptr, int col_actual_length, T &v) {
    switch (col_type) {
        case MYSQL_TYPE_TINY: { // 1 byte (char / tinyint)
            int8_t val = 0;
            std::memcpy(&val, data_ptr, 1);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_SHORT: { // 2 bytes (short / smallint)
            int16_t val = 0;
            std::memcpy(&val, data_ptr, 2);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_LONG: { // 4 bytes (int / mediumint)
            int32_t val = 0;
            std::memcpy(&val, data_ptr, 4);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_LONGLONG: { // 8 bytes (long long / bigint)
            int64_t val = 0;
            std::memcpy(&val, data_ptr, 8);
            v = static_cast<T>(val); // Note: Truncation could happen if bigint > INT_MAX
            break ;
        }
        case MYSQL_TYPE_FLOAT: { // 4 bytes float
            float val = 0.0f;
            std::memcpy(&val, data_ptr, 4);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_DOUBLE: { // 8 bytes double
            double val = 0.0;
            std::memcpy(&val, data_ptr, 8);
            v = static_cast<T>(val);
            break ;
        }
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING: {
            // Convienence fallback: string to int translation "123" -> 123
            std::string str(data_ptr, col_actual_length);
            v = str.empty() ? 0 : std::stod(str);
            break ;
        }
        default:
            throw std::runtime_error("Type Mismatch: Cannot implicitly convert column type to integer.");
    }
}

void MySQLQueryResultHandle::read(int idx, double &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0.0;
    
    const auto &col = columns_[idx] ;
    read_float(col.type_, col.buffer_.data(), col.actual_length_, val) ;
   
}

void MySQLQueryResultHandle::read(int idx, float &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0.0;
    
    const auto &col = columns_[idx] ;
    read_float(col.type_, col.buffer_.data(), col.actual_length_, val) ;
}

void MySQLQueryResultHandle::read(int idx, long long int &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
}

void MySQLQueryResultHandle::read(int idx, unsigned long long &val) const {
    check_idx(idx);
    if ( columns_[idx].is_null_ ) val = 0;
    
    const auto &col = columns_[idx] ;
    read_int(col.type_, col.buffer_.data(), col.actual_length_, val) ;
 
}

void MySQLQueryResultHandle::read(int idx, std::string &val) const {
    check_idx(idx) ;
    if ( columns_[idx].is_null_ ) val = "";
    val.assign(columns_[idx].buffer_.data(), columns_[idx].actual_length_);
}

void MySQLQueryResultHandle::read(int idx, Blob &result_buffer) const {
    check_idx(idx) ;
    const auto &col = columns_[idx] ;
    if ( col.is_null_ ) { return; }
    // Validate that the underlying column is actually a binary or string format
    if (col.type_ != MYSQL_TYPE_BLOB && col.type_ != MYSQL_TYPE_LONG_BLOB &&
        col.type_ != MYSQL_TYPE_MEDIUM_BLOB && col.type_ != MYSQL_TYPE_TINY_BLOB &&
        col.type_ != MYSQL_TYPE_STRING && col.type_ != MYSQL_TYPE_VAR_STRING) {
        throw Exception("Type Mismatch: Column is not a BLOB or binary type.");
    }

    unsigned long total_blob_size = col.actual_length_;

    // Check if the file payload fits entirely within our pre-allocated landing buffer
    if ( total_blob_size <= col.buffer_.size() ) {
        result_buffer.resize(total_blob_size);
        std::memcpy(result_buffer.data(), col.buffer_.data(), total_blob_size);
    } 
    else {
        // --- STREAMING FETCH FOR MASSIVE BLOBS ---
        // Resize our target container to perfectly fit the full database payload
        result_buffer.resize(total_blob_size);
        
        // Copy what we already captured in the initial fetch pass
        std::memcpy(result_buffer.data(), col.buffer_.data(), col.buffer_.size());
        
        // Fetch the remaining data chunks directly from the server pipeline
        unsigned long offset = col.buffer_.size();
        unsigned long remaining = total_blob_size - offset;

        MYSQL_BIND stream_bind{};
        std::memset(&stream_bind, 0, sizeof(MYSQL_BIND));
        stream_bind.buffer_type = col.type_;
        stream_bind.buffer = reinterpret_cast<char*>(result_buffer.data() + offset);
        stream_bind.buffer_length = remaining;

        // Fetch the rest of the column data into our output vector block
        if ( mysql_stmt_fetch_column(stmt_->handle(), &stream_bind, idx, offset) != 0) {
            throw Exception("Streaming BLOB retrieval failed: " + std::string(mysql_stmt_error(stmt_->handle())));
        }

    }
    
}
 

}



