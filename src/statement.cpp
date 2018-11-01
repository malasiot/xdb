#include <wspp/database/statement.hpp>
#include <wspp/database/connection.hpp>

#include <boost/algorithm/string.hpp>

using namespace std ;

namespace wspp { namespace db {

Statement::Statement(Connection &con, const std::string & sql) {
    con.check() ;
    stmt_ = con.handle()->createStatement(sql) ;
}

std::string escapeName(const std::string &unescaped) {
    string e = boost::algorithm::replace_all_copy(unescaped, "\"", "\"\"") ;
    return '"' + e + '"' ;
}

} // namespace db
} // namespace wspp
