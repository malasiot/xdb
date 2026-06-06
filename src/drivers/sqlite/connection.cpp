#include "connection.hpp"
#include "exceptions.hpp"
#include "statement.hpp"

using namespace std ;

namespace xdb {

void SQLiteConnectionHandle::close() {
    sqlite3_close(handle_) ;
}

StatementHandlePtr SQLiteConnectionHandle::createStatement(const char *sql)
{
    const char * tail = 0;

    sqlite3_stmt *stmt ;
    if ( sqlite3_prepare_v2(handle_, sql, -1, &stmt ,&tail) != SQLITE_OK )
        throw SQLiteException(handle_) ;

    return StatementHandlePtr(new SQLiteStatementHandle(stmt)) ;
}


void SQLiteConnectionHandle::exec(const char *sql, ...)
{
    va_list arguments ;
    va_start(arguments, sql);

    char *sql_e = sqlite3_vmprintf(sql, arguments) ;

    char *err_msg ;
    if ( sqlite3_exec(handle_, sql_e, NULL, NULL, &err_msg) != SQLITE_OK ) {
        string msg(err_msg) ;
        sqlite3_free(err_msg) ;

        throw Exception(msg) ;
    }

    sqlite3_free(sql_e) ;

    va_end(arguments);
}


void SQLiteConnectionHandle::begin() {
    exec("BEGIN") ;
}

void SQLiteConnectionHandle::commit() {
    exec("COMMIT") ;
}

void SQLiteConnectionHandle::rollback() {
    exec("ROLLBACK");
}

uint64_t SQLiteConnectionHandle::last_insert_rowid() const
{
    return sqlite3_last_insert_rowid(handle_) ;
}


}
