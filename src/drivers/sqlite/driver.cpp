#include "driver.hpp"
#include "connection.hpp"

using namespace std ;

namespace xdb {

ConnectionHandlePtr SQLiteDriver::open(const Dictionary &params) const {
    sqlite3 *handle ;

    int flags = 0 ;

    string database ;
    auto it = params.find("db") ;
    if ( it == params.end() ) return nullptr ;
    else database = it->second ;

    string mode ;
    it = params.find("mode") ;
    if ( it == params.end() ) mode = "rw" ;
    else mode = it->second ;

    if ( mode == "rw")
        flags |= SQLITE_OPEN_READWRITE ;
    else if ( mode == "r" )
        flags |= SQLITE_OPEN_READONLY ;
    else if ( mode == "rc")
    flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ;

    if ( sqlite3_open_v2(database.c_str(), &handle, flags, NULL)  != SQLITE_OK )
        return nullptr ;
    else
        return ConnectionHandlePtr(new SQLiteConnectionHandle(handle)) ;
}
}

