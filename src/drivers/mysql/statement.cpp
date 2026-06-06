#include "statement.hpp"
#include "exceptions.hpp"
#include "query_result.hpp"

#include <cstring>

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
    pending_blobs_.clear(); // Clear out streaming chunks
}

void MySQLStatementHandle::finalize()
{
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
        
    auto& buf = dynamic_buffers_.emplace_back(sizeof(bool));
    my_bool* null_indicator = reinterpret_cast<my_bool*>(buf.data());
    *null_indicator = true;
        
    b.buffer = nullptr;
    b.is_null = null_indicator; 
    binds_[idx] = b ;
    return *this ;
}


StatementHandle &MySQLStatementHandle::bind(int idx, unsigned char v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_TINY;
    
    std::vector<char> buf(sizeof(unsigned char)) ;
    std::memcpy(buf.data(), &v, sizeof(unsigned char));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();
    
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, char v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_TINY;
        
    std::vector<char> buf(sizeof(char)) ;
    std::memcpy(buf.data(), &v, sizeof(char));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, int v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_LONG;
   
    std::vector<char> buf(sizeof(int)) ;
    std::memcpy(buf.data(), &v, sizeof(int));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned int v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_LONG;
        
    std::vector<char> buf(sizeof(unsigned int)) ;
    std::memcpy(buf.data(), &v, sizeof(unsigned int));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned short int v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_SHORT;
        
    std::vector<char> buf(sizeof(unsigned short)) ;
    std::memcpy(buf.data(), &v, sizeof(unsigned short));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, short int v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_SHORT;
        
    std::vector<char> buf(sizeof(short)) ;
    std::memcpy(buf.data(), &v, sizeof(short));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, long int v) {
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_LONG;
        
    std::vector<char> buf(sizeof(long)) ;
    std::memcpy(buf.data(), &v, sizeof(long));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned long int v) {
   checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_LONG;
        
    std::vector<char> buf(sizeof(unsigned long)) ;
    std::memcpy(buf.data(), &v, sizeof(unsigned long));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, long long int v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_LONGLONG;
        
    std::vector<char> buf(sizeof(long long)) ;
    std::memcpy(buf.data(), &v, sizeof(long long));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, unsigned long long int v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_LONGLONG;
        
    std::vector<char> buf(sizeof(unsigned long long)) ;
    std::memcpy(buf.data(), &v, sizeof(unsigned long long));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, double v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_DOUBLE;
        
    std::vector<char> buf(sizeof(double)) ;
    std::memcpy(buf.data(), &v, sizeof(double));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, float v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_FLOAT;
        
    std::vector<char> buf(sizeof(float)) ;
    std::memcpy(buf.data(), &v, sizeof(float));
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();

    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, const string &v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_STRING;

    std::vector<char> buf(v.size() + 1) ;
    std::memcpy(buf.data(), v.c_str(), v.size() + 1);
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();
    b.buffer_length = v.size()  ;
        
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, const Blob &blob){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_BLOB;
    
    // Define an optimal 64KB threshold for inline processing
    const unsigned long STREAMING_THRESHOLD = 64 * 1024; 

    if ( blob.size() <= STREAMING_THRESHOLD ) {
        // --- METHOD A: INLINE BUFFER COPY (Fast for small BLOBs) ---

        std::vector<char> buf(blob.size()) ;
        std::memcpy(buf.data(), blob.data(), blob.size());
        dynamic_buffers_[idx] = std::move(buf) ;
        b.buffer = dynamic_buffers_[idx].data();
        b.buffer_length = blob.size();
    } 
    else {
        // --- METHOD B: NETWORK STREAMING (Safe for large BLOBs) ---
        b.buffer = nullptr; // Tells MySQL to wait for chunks
        
        pending_blobs_.push_back({idx, blob});
    }
    
    return *this ;
}

StatementHandle &MySQLStatementHandle::bind(int idx, const char *v){
    checkBindIdx(idx) ;
    MYSQL_BIND& b = binds_[idx];
    b.buffer_type = MYSQL_TYPE_STRING;

    int len = strlen(v) ;
        
    std::vector<char> buf(len + 1) ;
    std::memcpy(buf.data(), v, len + 1);
    dynamic_buffers_[idx] = std::move(buf) ;
    b.buffer = dynamic_buffers_[idx].data();
    b.buffer_length = len ;
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

   /*  for (const auto& blob : pending_blobs_) {
            unsigned long offset = 0;
            while (offset < blob.stream_.size) {
                unsigned long send_bytes = std::min(16384L, static_cast<long>(blob.stream.size - offset));
                if (mysql_stmt_send_long_data(stmt_, blob.param_index, blob.stream.data + offset, send_bytes) != 0) {
                    throw std::runtime_error("BLOB chunk stream failed.");
                }
                offset += send_bytes;
            }
        }
*/
    if ( mysql_stmt_execute(handle_) != 0 ) {
        throw Exception(mysql_stmt_error(handle_));
    }

   //     sqlite3_step(handle_) ;

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

