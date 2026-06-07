#ifndef XDB_TRANSACTION_HPP
#define XDB_TRANSACTION_HPP

#include <xdb/connection_handle.hpp>

#include <string>

namespace xdb {

class Connection ;

class Transaction
{
public:

    explicit Transaction(Connection &con_); // the constructor starts the constructor
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    Transaction(Transaction&& other) noexcept ;
    ~Transaction() ;

    void commit();
    void rollback();

private:

    ConnectionHandlePtr con_ ;
    bool committed_ = false ;
};

} // namespace xdb



#endif
