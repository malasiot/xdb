#ifndef __PGSQL_EXCEPTIONS_HPP__
#define __PGSQL_EXCEPTIONS_HPP__

#include <wspp/database/exception.hpp>

#include <libpq-fe.h>

namespace wspp { namespace db {

class PGSQLException: public Exception {
public:
    PGSQLException(PGconn *handle): Exception(PQerrorMessage(handle)) {}
    PGSQLException(PGresult *handle): Exception(PQresultErrorMessage(handle)) {
        PQclear(handle) ;
    }
};

} // namespace db
} // namespace wspp

#endif
