#ifndef __DATABASE_QUERY_HPP__
#define __DATABASE_QUERY_HPP__

#include <xdb/statement.hpp>
#include <xdb/query_result.hpp>
#include <string>

namespace xdb {

class Connection ;
class QueryResult ;

class Query: public Statement {
public:
    Query(Connection &con, const char *sql) ;

    template<typename ...Args>
    Query(Connection& con, const char *sql, Args&&... args): Query(con, sql) {
        bindAll(std::forward<Args>(args)...) ;
    }

    QueryResult exec() ;

    template<typename ...Args>
    QueryResult operator()(Args&&... args) {
        bindAll(std::forward<Args>(args)...) ;
        return exec() ;
    }

    QueryResult operator()() {
        return exec() ;
    }
};

} // namespace xdb


#endif
