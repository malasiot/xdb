#include <xdb/statement.hpp>
#include <xdb/connection.hpp>

using namespace std ;

namespace xdb {

Statement::Statement(Connection &con, const std::string & sql) {
    con.check() ;
    stmt_ = con.handle()->createStatement(sql) ;
}

std::string escapeName(const std::string &unescaped) {
    string escaped(unescaped) ;
    size_t pos ;
    while ( (pos = unescaped.find('"', pos)) != string::npos ) {
        escaped.replace(pos, 1, "\"\"");
        pos += 2;
    }

    return '"' + escaped + '"' ;
}

} // namespace xdb

