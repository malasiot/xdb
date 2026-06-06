#ifndef XDB_SQLITE_EXCEPTIONS_HPP
#define XDB_SQLITE_EXCEPTIONS_HPP

#include <xdb/exception.hpp>

#include <sqlite3.h>

namespace xdb {

class SQLiteException: public Exception {
public:
    SQLiteException(sqlite3 *handle): Exception(sqlite3_errmsg(handle)) {}
};

} // namespace db


#endif
