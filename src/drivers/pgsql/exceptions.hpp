#ifndef XDB_PGSQL_EXCEPTIONS_HPP
#define XDB_PGSQL_EXCEPTIONS_HPP

#include <xdb/exception.hpp>

#include <libpq-fe.h>

namespace xdb {

class PGSQLException: public Exception {
public:
    PGSQLException(PGconn *handle): Exception(PQerrorMessage(handle)) {}
    PGSQLException(PGresult *handle): Exception(PQresultErrorMessage(handle)) {
        PQclear(handle) ;
    }
};

} // namespace xdb


#endif
