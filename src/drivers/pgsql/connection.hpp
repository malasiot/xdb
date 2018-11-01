#ifndef __DATABASE_PGSQL_CONNECTION_HPP__
#define __DATABASE_PGSQL_CONNECTION_HPP__

#include <libpq-fe.h>

#include <wspp/database/connection_handle.hpp>

namespace wspp { namespace db {

class PGSQLConnectionHandle: public ConnectionHandle {
public:
    PGSQLConnectionHandle(PGconn *handle): handle_(handle) {}
    ~PGSQLConnectionHandle() { close() ; }

    void close() override ;

    StatementHandlePtr createStatement(const std::string &sql) ;

    void begin() override ;
    void commit() override ;
    void rollback() override ;

    uint64_t last_insert_rowid() const override ;

private:

    void exec(const char *sql);

    PGconn *handle_ ;
};



} // namespace db
} // namespace wspp

#endif
