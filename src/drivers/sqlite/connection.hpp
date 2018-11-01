#ifndef __DATABASE_SQLITE_DRIVER_CONNECTION_HPP__
#define __DATABASE_SQLITE_DRIVER_CONNECTION_HPP__

#include <sqlite3.h>

#include <wspp/database/connection_handle.hpp>

namespace wspp { namespace db {

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



} // namespace db
} // namespace wspp

#endif
