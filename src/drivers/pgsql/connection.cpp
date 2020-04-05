#include "connection.hpp"
#include "exceptions.hpp"
#include "statement.hpp"

using namespace std ;

namespace xdb {

void PGSQLConnectionHandle::close() {
    PQfinish(handle_) ;
}

StatementHandlePtr PGSQLConnectionHandle::createStatement(const std::string &sql)
{
    return StatementHandlePtr(new PGSQLStatementHandle(sql, handle_)) ;

}


void PGSQLConnectionHandle::exec(const char *sql)
{
    PGresult *res = PQexec(handle_, sql);

    if ( PQresultStatus(res) != PGRES_COMMAND_OK ) {
        PQclear(res);
        throw PGSQLException(handle_) ;
    }

    PQclear(res);
}


void PGSQLConnectionHandle::begin() {
    exec("BEGIN") ;
}

void PGSQLConnectionHandle::commit() {
    exec("COMMIT") ;
}

void PGSQLConnectionHandle::rollback() {
    exec("ROLLBACK");
}

uint64_t PGSQLConnectionHandle::last_insert_rowid() const
{
    //return sqlite3_last_insert_rowid(handle_) ;
    return 0 ;
}


}
