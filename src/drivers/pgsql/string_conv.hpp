#ifndef XDB_PGSQL_STRING_CONV_HPP
#define XDB_PGSQL_STRING_CONV_HPP

#include <xdb/types.hpp>
#include <string>
#include <sstream>
#include <cstring>

namespace xdb {

template<typename T>
inline std::string pq_to_string(T val) {
    return std::to_string(val) ;
}

template<> inline std::string pq_to_string(const char *val) { return val; }

template<> inline std::string pq_to_string(const std::string &val) { return val; }

template<> inline std::string pq_to_string(std::string val) { return val; }

template<> inline std::string pq_to_string(char *val) { return val; }

template<> inline std::string pq_to_string(bool val) { return (val) ? "true" : "false"; }

template<typename T>
inline bool pq_from_string(const char *str, T &val) {
    std::istringstream strm(str) ;
    strm >> val ;
    return strm.good() ;
}

template<>
inline bool pq_from_string(const char *str, bool &val) {

    bool OK, result=false;

    switch ( *str )
    {
    case 0:
        val = false;
        return true ;
    case 'f':
    case 'F':
        if ( strcmp(str + 1, "alse") == 0 ||
             strcmp(str + 1, "ALSE") == 0 ) {
            val = false ;
            return true ;
        }
        break;
    case '0': {
        int ival ;
        if ( pq_from_string(str, ival) && ival == 0 || ival == 1) {
            val = ( ival != 0 );
            return  true ;
        }
    }
        break;
    case '1':
        if ( str[1] ) {
            val = true ;
            return true ;
        }
        break;
    case 't':
    case 'T':
        if ( strcmp(str + 1, "rue") == 0 ||
             strcmp(str + 1, "RUE") == 0 ) {
            val = false ;
            return true ;
        }
        break;
    default:
        return false ;
    }

    return false ;
}

} // namespace xdb

#endif
