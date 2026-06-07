#ifndef __SQLITE_STATEMENT_HPP__
#define __SQLITE_STATEMENT_HPP__

#include <mysql.h>

#include <xdb/statement_handle.hpp>

namespace xdb {

class MySQLStatementHandle final: public StatementHandle, public std::enable_shared_from_this<MySQLStatementHandle> {
public:
    MySQLStatementHandle(const char *sql, MYSQL *con) ;
    ~MySQLStatementHandle() {
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

    MYSQL_STMT *handle() const { return handle_ ; }

    uint64_t lastInsertId() const {
        return static_cast<uint64_t>(mysql_stmt_insert_id(handle_));
    }
private:
    void checkBindIdx(int) ;
    
    struct BlobPayload {
        int param_index_ ;
        const Blob &stream_;
    };
    std::vector<BlobPayload> pending_blobs_;

    MYSQL_STMT *handle_ ;
    std::vector<MYSQL_BIND> binds_;
    std::vector<std::vector<char>> dynamic_buffers_ ;
    std::unordered_map<int, unsigned long> length_buffers_ ; 
   
    void check() const;
};



} // namespace xdb


#endif
