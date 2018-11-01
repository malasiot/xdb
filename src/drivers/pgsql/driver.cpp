#include "driver.hpp"
#include "connection.hpp"

using namespace std ;


namespace xdb {

static string escape_param_value(const string &src) {
    string dst ;

    if ( src.empty() ) return "''" ;

    bool has_spaces = false ;
    for( const auto &c: src ) {
        if ( isspace(c) ) has_spaces = true ;
        switch ( c ) {
        case ' ':
            has_spaces = true ;
            dst.push_back(c) ;
            break ;
        case '\'':
            dst.append("\\'") ;
            break ;
        case '\\':
            dst.append("\\\\") ;
            break ;
        default:
            dst.push_back(c) ;
        }
    }

    if ( has_spaces  ) return "'" + dst + "'" ;
    else return dst ;
}

ConnectionHandlePtr PGSQLDriver::open(const Dictionary &params) const {

    string conn_info ;

    for( const auto &p: params ) {
        const string &key = p.first, &val = p.second ;
        if ( !conn_info.empty() ) conn_info.push_back(' ') ;
            conn_info.append(key) ;
            conn_info.push_back('=') ;
            conn_info.append(escape_param_value(val)) ;
    }

    PGconn *handle = PQconnectdb(conn_info.c_str());

    /* Check to see that the backend connection was successfully made */
    if ( PQstatus(handle) != CONNECTION_OK ) {
        cout << PQerrorMessage(handle) << endl ;
        PQfinish(handle) ;
        return nullptr ;
    }
    else return ConnectionHandlePtr(new PGSQLConnectionHandle(handle)) ;
}
}

