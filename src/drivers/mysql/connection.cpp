#include "connection.hpp"
#include "exceptions.hpp"
#include "statement.hpp"

#include <cassert>

using namespace std ;

namespace xdb {

void MySQLConnectionHandle::close() {
    assert(handle_) ;
    mysql_close(handle_) ;
}

StatementHandlePtr MySQLConnectionHandle::createStatement(const char *sql)
{
    return StatementHandlePtr(new MySQLStatementHandle(sql, handle_)) ;
}


void MySQLConnectionHandle::exec(const char *sql, ...)
{
  /*  va_list arguments ;
    va_start(arguments, sql);

    char *sql_e = sqlite3_vmprintf(sql, arguments) ;

    char *err_msg ;
    if ( sqlite3_exec(handle_, sql_e, NULL, NULL, &err_msg) != SQLITE_OK ) {
        string msg(err_msg) ;
        sqlite3_free(err_msg) ;

        throw Exception(msg) ;
    }

    sqlite3_free(sql_e) ;

    va_end(arguments);*/
}


void MySQLConnectionHandle::begin() {
    exec("BEGIN") ;
}

void MySQLConnectionHandle::commit() {
    exec("COMMIT") ;
}

void MySQLConnectionHandle::rollback() {
    exec("ROLLBACK");
}

uint64_t MySQLConnectionHandle::last_insert_rowid() const
{
   // return sqlite3_last_insert_rowid(handle_) ;
}


}
