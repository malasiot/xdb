#ifndef XDB_SQLITE_EXCEPTIONS_HPP
#define XDB_SQLITE_EXCEPTIONS_HPP

#include <xdb/exception.hpp>

#include <sqlite3.h>

<<<<<<< HEAD
namespace cvx { namespace db {
=======
namespace xdb {
>>>>>>> 8b21f6742f70ee3f2566dd657a827a8c7ca70f93

class SQLiteException: public Exception {
public:
    SQLiteException(sqlite3 *handle): Exception(sqlite3_errmsg(handle)) {}
};

} // namespace db


#endif
