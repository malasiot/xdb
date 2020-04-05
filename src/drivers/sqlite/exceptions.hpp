#ifndef __SQLITE_EXCEPTIONS_HPP__
#define __SQLITE_EXCEPTIONS_HPP__

#include <wspp/database/exception.hpp>

#include <sqlite3.h>

namespace cvx { namespace db {

class SQLiteException: public Exception {
public:
    SQLiteException(sqlite3 *handle): Exception(sqlite3_errmsg(handle)) {}
};

} // namespace db
} // namespace wspp

#endif
