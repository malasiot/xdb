#include <xdb/query.hpp>
#include <xdb/connection.hpp>

using namespace std ;
namespace xdb {

Query::Query(Connection &con, const string &sql):
    Statement(con, sql) {
}

QueryResult Query::exec()
{
    return stmt_->execQuery() ;
}


} // namespace xdb
