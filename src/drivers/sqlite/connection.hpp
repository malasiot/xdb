#ifndef XDB_SQLITE_DRIVER_CONNECTION_HPP
#define XDB_SQLITE_DRIVER_CONNECTION_HPP

#include <sqlite3.h>

#include <xdb/connection_handle.hpp>

namespace xdb {

class SQLiteConnectionHandle: public ConnectionHandle {
public:
    SQLiteConnectionHandle(sqlite3 *handle): handle_(handle) {}
    ~SQLiteConnectionHandle() { close() ; }

    void close() override ;

    StatementHandlePtr createStatement(const std::string &sql) ;

    void begin() override ;
    void commit() override ;
    void rollback() override ;

    uint64_t last_insert_rowid() const override ;

private:

    void exec(const std::string &sql...);

    sqlite3 *handle_ ;
};



} // namespace xdb


#endif
