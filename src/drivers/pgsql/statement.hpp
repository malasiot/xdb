#ifndef XDB_PGSQL_STATEMENT_HPP
#define XDB_PGSQL_STATEMENT_HPP

#include <libpq-fe.h>

#include <xdb/statement_handle.hpp>

#include "parameters.hpp"

namespace xdb {

class PGSQLStatementHandle final: public StatementHandle, public std::enable_shared_from_this<PGSQLStatementHandle> {
public:
    PGSQLStatementHandle(const std::string &sql, PGconn *handle): sql_(sql), handle_(handle) {}

    ~PGSQLStatementHandle() {
        finalize() ;
    }

    void clear() override ;

    void finalize() override ;

    StatementHandle &bind(int idx, const NullType &) override ;
    StatementHandle &bind(int idx, unsigned char v) override;
    StatementHandle &bind(int idx, char v) override;
    StatementHandle &bind(int idx, unsigned short v) override;
    StatementHandle &bind(int idx, short v) override;
    StatementHandle &bind(int idx, unsigned long v) override;
    StatementHandle &bind(int idx, long v) override;
    StatementHandle &bind(int idx, unsigned int v) override;
    StatementHandle &bind(int idx, int v) override;
    StatementHandle &bind(int idx, unsigned long long int v) override;
    StatementHandle &bind(int idx, long long int v) override;
    StatementHandle &bind(int idx, double v) override;
    StatementHandle &bind(int idx, float v) override;
    StatementHandle &bind(int idx, const std::string &v) override;
    StatementHandle &bind(int idx, const Blob &blob) override;

    StatementHandle &bind(int idx, const char *str) override ;

    int placeholderNameToIndex(const std::string &name) override;

    void exec() override ;
    QueryResult execQuery() override ;

    PGconn *handle() const { return handle_ ; }
private:

    PGconn *handle_ ;

    void check() const;
    void prepare() ;
    PGresult *doExec() ;
    bool checkResult(PGresult *) const ;

    PreparedStatementParameters params_ ;
    std::string sql_, name_ ;
};



} // namespace db


#endif
