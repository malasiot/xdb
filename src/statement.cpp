#include <xdb/statement.hpp>
#include <xdb/connection.hpp>

using namespace std ;

<<<<<<< HEAD
namespace cvx { namespace db {
=======
namespace xdb {
>>>>>>> 8b21f6742f70ee3f2566dd657a827a8c7ca70f93

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

