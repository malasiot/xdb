#ifndef XDB_MYSQL_DRIVER_HPP
#define XDB_MYSQL_DRIVER_HPP

#include <memory>
#include <map>

namespace xdb {

class ConnectionHandle ;

// The Mysql driver
//
// The connections string is of the form mysql:host=<host>;user=<user>;password=<password>;db=<db_name>[;port=<port>]
//


class MySQLDriver {
using Dictionary = std::map<std::string, std::string>;
public:

    MySQLDriver() = default ;

    static const MySQLDriver &instance() {
        static MySQLDriver instance ;
        return instance ;
    }

    std::shared_ptr<ConnectionHandle> open(const Dictionary &params) const ;
};


}




#endif
