#include "connection.hpp"
#include "statement.hpp"

#include <cassert>

using namespace std ;

namespace xdb {

void MySQLConnectionHandle::close() {
    assert(handle_) ;
    mysql_close(handle_) ;
}

StatementHandlePtr MySQLConnectionHandle::createStatement(const char *sql) {
    return StatementHandlePtr(new MySQLStatementHandle(sql, handle_)) ;
}


void MySQLConnectionHandle::begin() {
    if ( mysql_query(handle_, "START TRANSACTION") != 0 ) {
        throw xdb::Exception(mysql_error(handle_));
    }
}

void MySQLConnectionHandle::commit() {
    if ( mysql_query(handle_, "COMMIT") != 0 ) {
        throw xdb::Exception(mysql_error(handle_));
    }
}

void MySQLConnectionHandle::rollback() {
    if ( mysql_query(handle_, "ROLLBACK") != 0 ) {
        throw xdb::Exception(mysql_error(handle_));
    }
}

uint64_t MySQLConnectionHandle::last_insert_rowid() const {
    return static_cast<uint64_t>(mysql_insert_id(handle_)); 
}


}
