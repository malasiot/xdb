#ifndef XDB_PGSQL_DRIVER_HPP
#define XDB_PGSQL_DRIVER_HPP

#include <memory>
#include <xdb/dictionary.hpp>

namespace xdb {

class ConnectionHandle ;

// The PostgreSQL driver
//
// The connections string is of the form pgsql:<; delimited option list>
//
// for available options see:
// https://www.postgresql.org/docs/9.4/static/libpq-connect.html

class PGSQLDriver {

public:

    PGSQLDriver() = default ;

    static const PGSQLDriver &instance() {
        static PGSQLDriver instance ;
        return instance ;
    }

    std::shared_ptr<ConnectionHandle> open(const Dictionary &params) const ;
};

}

#endif
