#ifndef XDB_TRANSACTION_HPP
#define XDB_TRANSACTION_HPP

#include <xdb/connection_handle.hpp>

#include <string>

namespace xdb {

class Connection ;

class Transaction
{
public:

    Transaction(Connection &con_); // the constructor starts the constructor

    // you should explicitly call commit or rollback to close it

    void commit();
    void rollback();

private:

    ConnectionHandlePtr con_ ;
};

} // namespace xdb



#endif
