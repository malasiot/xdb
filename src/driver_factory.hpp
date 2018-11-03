#ifndef XDB_DRIVER_FACTORY_HPP
#define XDB_DRIVER_FACTORY_HPP

#include <string>

#include <xdb/connection.hpp>

namespace xdb {

class DriverFactory {
    using Dictionary = std::map<std::string, std::string>;
public:

    DriverFactory() = default ;

    // open connection to database with given params
    // dsn is a connection string or uri link to local file (similar to PHP PDO)

    std::shared_ptr<ConnectionHandle> createConnection(const std::string &dsn) const ;

    static const DriverFactory &instance() {
        static DriverFactory factory ;
        return factory ;
    }

    static bool parseParamString(const std::string &str, Dictionary &params) ;

};


} // namespace xdb



#endif
