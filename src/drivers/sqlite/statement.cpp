#include "statement.hpp"
#include "exceptions.hpp"
#include "query_result.hpp"

#include <cstring>

using namespace std ;

namespace cvx { namespace db {


void SQLiteStatementHandle::check() const {
    if ( !handle_ )
        throw Exception("Statement has not been compiled.");
}

void SQLiteStatementHandle::clear() {
    check();
    if ( sqlite3_reset(handle_) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
}

void SQLiteStatementHandle::finalize()
{
    check();
    if ( sqlite3_finalize(handle_) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
}


StatementHandle &SQLiteStatementHandle::bind(int idx, const NullType &) {

    check();
    if ( sqlite3_bind_null(handle_, idx) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}


StatementHandle &SQLiteStatementHandle::bind(int idx, unsigned char v) {
    check();
    if ( sqlite3_bind_int(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, char v) {
    check();
    if ( sqlite3_bind_int(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, int v) {
    check();
    if ( sqlite3_bind_int(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, unsigned int v) {
    check();
    if ( sqlite3_bind_int(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, unsigned short int v) {
    check();
    if ( sqlite3_bind_int(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, short int v) {
    check();
    if ( sqlite3_bind_int(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, long int v) {
    check();
    if ( sqlite3_bind_int64(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, unsigned long int v) {
    check();
    if ( sqlite3_bind_int64(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, long long int v){
    check();
    if ( sqlite3_bind_int64(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, unsigned long long int v){
    check();
    if ( sqlite3_bind_int64(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, double v){
    check() ;
    if ( sqlite3_bind_double(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, float v){
    check() ;
    if ( sqlite3_bind_double(handle_, idx, v) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, const string &v){
    check() ;
    if ( sqlite3_bind_text(handle_, idx, v.c_str(), int(v.size()), SQLITE_TRANSIENT ) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, const Blob &blob){
    check() ;
    if ( sqlite3_bind_blob(handle_, idx, blob.data(), blob.size(), SQLITE_TRANSIENT ) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

StatementHandle &SQLiteStatementHandle::bind(int idx, const char *v){
    check() ;
    if ( sqlite3_bind_text(handle_, idx, v, strlen(v), SQLITE_TRANSIENT ) != SQLITE_OK )
        throw SQLiteException(sqlite3_db_handle(handle_));
    return *this ;
}

int SQLiteStatementHandle::placeholderNameToIndex(const std::string &name) {
    int idx = sqlite3_bind_parameter_index(handle_, name.c_str() );
    if ( idx ) return idx ;
    else throw Exception(name + " is not a valid statement placeholder") ;
}

void SQLiteStatementHandle::exec()
{

        check() ;

        sqlite3_step(handle_) ;

}

QueryResult SQLiteStatementHandle::execQuery()
{

    return QueryResult(QueryResultHandlePtr(new SQLiteQueryResultHandle(shared_from_this()))) ;
}

} // namespace db
} // namespace wspp
