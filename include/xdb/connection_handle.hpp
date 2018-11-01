#ifndef XDB_CONNECTION_HANDLE_HPP
#define XDB_CONNECTION_HANDLE_HPP

#include <memory>
#include <xdb/statement_handle.hpp>

namespace xdb {

class ConnectionHandle {
public:
    ConnectionHandle() = default ;

    virtual ~ConnectionHandle() {}
    virtual void close() = 0 ;
    virtual StatementHandlePtr createStatement(const std::string &sql) = 0;

    virtual void begin() = 0 ;
    virtual void commit() = 0 ;
    virtual void rollback() = 0 ;

    virtual uint64_t last_insert_rowid() const = 0 ;
} ;

typedef std::shared_ptr<ConnectionHandle> ConnectionHandlePtr ;


}

#endif
