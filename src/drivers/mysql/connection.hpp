#ifndef XDB_MYSQL_DRIVER_CONNECTION_HPP
#define XDB_MYSQL_DRIVER_CONNECTION_HPP

#include <mysql.h>

#include <xdb/connection_handle.hpp>

namespace xdb {

class MySQLConnectionHandle: public ConnectionHandle {
public:
    MySQLConnectionHandle(MYSQL *handle): handle_(handle) {}
    ~MySQLConnectionHandle() { close() ; }

    void close() override ;

    StatementHandlePtr createStatement(const char *sql) override ;

    void begin() override ;
    void commit() override ;
    void rollback() override ;

    uint64_t last_insert_rowid() const override ;

private:

    void exec(const char *sql...);

    MYSQL *handle_ ;
};



} // namespace xdb


#endif
