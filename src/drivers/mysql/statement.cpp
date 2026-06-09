#include "statement.hpp"
#include "query_result.hpp"

#include <cstring>
#include <algorithm>

using namespace std ;

namespace xdb {

void MySQLStatementHandle::check() const {
    if ( !handle_ )
        throw Exception("Statement has not been compiled.");
}

void MySQLStatementHandle::clear() {
    check();
    binds_.clear();
    dynamic_buffers_.clear();
    pending_blobs_.clear(); 
    length_buffers_.clear() ;
}

void MySQLStatementHandle::finalize() {
    check();
    mysql_stmt_close(handle_);
}

void MySQLStatementHandle::checkBindIdx(int idx) {
    check() ;
    if ( idx < 0 || idx >= binds_.size() ) {
        throw Exception("Trying to bind parameter to index " + std::to_string(idx) + " which is out of bounds" ) ;
    } 
}


StatementHandle &MySQLStatementHandle::bind(int idx, const NullType &) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_NULL;
        
    std::vector<char> buf(sizeof(bool)) ;
    dynamic_buffers_[idx] = std::move(buf) ;
    my_bool* null_indicator = reinterpret_cast<my_bool*>(dynamic_buffers_[idx].data());
    *null_indicator = true;
        
    b.buffer = nullptr;
    b.is_null = null_indicator; 
  
    return *this ;
}

template<class T>
void bind_primitive(T v, enum_field_types btype, MYSQL_BIND &b, std::vector<char> &storage) {
    b.buffer_type = btype;
    std::vector<char> buf(sizeof(T)) ;
    std::memcpy(buf.data(), &v, sizeof(T));
    storage = std::move(buf) ;
    b.buffer = storage.data();
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned char v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_TINY, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, char v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_TINY, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, int v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_LONG, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned int v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_LONG, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned short int v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_SHORT, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, short int v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_SHORT, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, long int v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_LONG, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned long int v) {
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_LONG, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, long long int v){
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_LONGLONG, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned long long int v){
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_LONGLONG, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, double v){
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_DOUBLE, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, float v){
    checkBindIdx(idx) ;
    bind_primitive(v, MYSQL_TYPE_FLOAT, binds_[idx], dynamic_buffers_[idx]);
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, const string &v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_STRING;

    length_buffers_[idx] = static_cast<unsigned long>(v.size());

    std::vector<char> buf(v.size()) ;
    memmove(buf.data(), v.data(), v.size()) ;
    dynamic_buffers_[idx] = std::move(buf) ;

    b.buffer = dynamic_buffers_[idx].data() ;
    b.length =  &length_buffers_[idx];
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, const Blob &blob){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_BLOB;
    
    // Define an optimal 64KB threshold for inline processing
    const unsigned long STREAMING_THRESHOLD = 64 * 1024; 

    if ( blob.size() <= STREAMING_THRESHOLD ) {
        std::vector<char> buf(sizeof(unsigned long)) ;
        dynamic_buffers_[idx] = std::move(buf) ;
        unsigned long* blob_sz = reinterpret_cast<unsigned long*>(dynamic_buffers_[idx].data());
        *blob_sz = blob.size() ;
        b.buffer = const_cast<char *>(reinterpret_cast<const char*>(blob.data()));
        b.length = blob_sz ;
        b.is_null = nullptr; 
    } 
    else { // handling of large blobs
        b.buffer = nullptr; // Tells MySQL to wait for chunks
        b.buffer_length = 0 ;
        b.length = nullptr;
        b.is_null = nullptr;
        pending_blobs_.push_back({idx, blob});
        
    }
    
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, const char *v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_STRING;

    int len = strlen(v) ;
    length_buffers_[idx] = static_cast<unsigned long>(len);

    std::vector<char> buf(len) ;
    memmove(buf.data(), v, len) ;
    dynamic_buffers_[idx] = std::move(buf) ;

    b.buffer = dynamic_buffers_[idx].data() ;
    b.length =  &length_buffers_[idx];
    return *this ;
}

int MySQLStatementHandle::placeholderNameToIndex(const std::string &name) {
    throw Exception("named placeholders are not supported");
    return -1 ;
}

MySQLStatementHandle::MySQLStatementHandle(const char *sql, MYSQL *con) {
    handle_ = mysql_stmt_init(con);
    if ( !handle_ ) {
        throw Exception("Failed to initialize statement structure");
    }
        
    if ( mysql_stmt_prepare(handle_, sql, strlen(sql)) != 0 ) {
        std::string err = mysql_stmt_error(handle_);
        mysql_stmt_close(handle_);
        handle_ = nullptr ;
        throw Exception("SQL Prepare Failed: " + err);
    }

    unsigned long param_count = mysql_stmt_param_count(handle_);
    binds_.resize(param_count);
    dynamic_buffers_.resize(param_count);
    std::memset(binds_.data(), 0, binds_.size() * sizeof(MYSQL_BIND));
}

void MySQLStatementHandle::exec() {
    check() ;

    if ( !binds_.empty() ) {
        if (mysql_stmt_bind_param(handle_, binds_.data()) != 0 ) {
            throw Exception(mysql_stmt_error(handle_));
        }
    }

    const size_t chunk_sz = 16384L ;

    for (const auto& blob : pending_blobs_) {
        unsigned long offset = 0;
        while ( offset < blob.stream_.size() ) {
            unsigned long send_bytes = std::min(chunk_sz, static_cast<size_t>(blob.stream_.size() - offset));
            if ( mysql_stmt_send_long_data(handle_, blob.param_index_, (const char *)blob.stream_.data() + offset, send_bytes) != 0 ) {
                throw std::runtime_error("BLOB chunk stream failed.");
            }
            offset += send_bytes;
        }

        offset = 0;
    }

    if ( mysql_stmt_execute(handle_) != 0 ) {
        throw Exception(mysql_stmt_error(handle_));
    }
}

QueryResult MySQLStatementHandle::execQuery()
{
    check() ;

    if ( !binds_.empty() ) {
        if (mysql_stmt_bind_param(handle_, binds_.data()) != 0 ) {
            throw Exception(mysql_stmt_error(handle_));
        }
    }

    if ( mysql_stmt_execute(handle_) != 0 ) {
        throw Exception(mysql_stmt_error(handle_));
    }

    QueryResultHandlePtr res(new MySQLQueryResultHandle(shared_from_this())) ;

    if (mysql_stmt_store_result(handle_) != 0) {
        throw std::runtime_error(mysql_stmt_error(handle_));
    }

    return res ;
}

} // namespace xdb

