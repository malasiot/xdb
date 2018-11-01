#ifndef XDB_EXCEPTION_HPP
#define XDB_EXCEPTION_HPP

#include <sqlite3.h>
#include <string>
#include <stdexcept>

namespace xdb {

class ConnectionHandle ;

class Exception: public std::runtime_error
{
public:
    Exception(const std::string &msg) ;
    Exception(ConnectionHandle &h) ;
};


} // namespace xdb



#endif
