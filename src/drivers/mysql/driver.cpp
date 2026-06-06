#include "driver.hpp"
#include "connection.hpp"

#include <mysql.h>

using namespace std ;

namespace xdb {

ConnectionHandlePtr MySQLDriver::open(const Dictionary &params) const {
    MYSQL *handle ;

    string database ;
    auto it = params.find("db") ;
    if ( it == params.end() ) return nullptr ;
    else database = it->second ;

    string host ;
    it = params.find("host") ;
    if ( it == params.end() ) host = "localhost" ;
    else host = it->second ;

    string username ;
    it = params.find("username") ;
    if ( it != params.end() ) 
        username = it->second ;

    string password ;
    it = params.find("password") ;
    if ( it != params.end() ) 
        password = it->second ;

    uint port ;
    it = params.find("port") ;
    if ( it == params.end() ) port = 3306 ;
    else port = stoul(it->second) ;

    handle = mysql_init(nullptr);
    if ( !handle ) {
        throw Exception("Failed to initialize MySQL/MariaDB");
        return nullptr ;
    }
    // Establish connection via the standard unified C-API call
    if ( !mysql_real_connect(handle, host.c_str(), username.c_str(), password.c_str(), 
                                database.empty() ? nullptr : database.c_str(), port, nullptr, 0)) {
        throw Exception(mysql_error(handle));
    }
    
    return ConnectionHandlePtr(new MySQLConnectionHandle(handle)) ;
}
}

