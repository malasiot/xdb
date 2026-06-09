#include "driver.hpp"
#include "connection.hpp"

#include <sstream>
using namespace std ;

namespace xdb {

static bool load_extensions(sqlite3 *handle, const std::vector<std::string> &extensions) {
    if ( extensions.empty() ) return true ;

    int rc = sqlite3_db_config(handle, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, 1, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to enable extensions: %s\n", sqlite3_errmsg(handle));
        sqlite3_close(handle);
        return false;
    }
   
    for ( const auto &ext: extensions ) {
        char *err_msg = NULL;
        rc = sqlite3_load_extension(handle, ext.c_str(), NULL, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to load SpatiaLite: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(handle);
            return false;
        }
    }

    return true ;
}

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

    vector<string> extensions ;
    it = params.find("ext");
    if ( it != params.end() ) {
        stringstream strm(it->second);
        string ext ;
        while(std::getline(strm, ext, ','))
            extensions.push_back(ext);
    }
    

    if ( mode == "rw")
        flags |= SQLITE_OPEN_READWRITE ;
    else if ( mode == "r" )
        flags |= SQLITE_OPEN_READONLY ;
    else if ( mode == "rc")
    flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ;

    if ( sqlite3_open_v2(database.c_str(), &handle, flags, NULL)  != SQLITE_OK )
        return nullptr ;
    else {
        if ( !load_extensions(handle, extensions) ) return nullptr ;
        return ConnectionHandlePtr(new SQLiteConnectionHandle(handle)) ;
    }

    
}


}

