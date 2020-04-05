#include <wspp/database/query.hpp>
#include <wspp/database/connection.hpp>

using namespace std ;
namespace cvx { namespace db {

Query::Query(Connection &con, const string &sql):
    Statement(con, sql) {
}

QueryResult Query::exec()
{
    return stmt_->execQuery() ;
}


} // namespace util
} // namespace wspp
