#ifndef __SQLITE_STATEMENT_HPP__
#define __SQLITE_STATEMENT_HPP__

#include <sqlite3.h>

#include <xdb/statement_handle.hpp>

namespace xdb {

class SQLiteStatementHandle final: public StatementHandle, public std::enable_shared_from_this<SQLiteStatementHandle> {
public:
    SQLiteStatementHandle(sqlite3_stmt *handle): handle_(handle) {}

    ~SQLiteStatementHandle() {
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

    sqlite3_stmt *handle() const { return handle_ ; }
private:

    sqlite3_stmt *handle_ ;
    std::map<std::string, int> field_map_ ;

    void check() const;
};



} // namespace xdb


#endif
