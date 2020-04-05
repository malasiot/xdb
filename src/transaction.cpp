#include <xdb/transaction.hpp>
#include <xdb/connection.hpp>

using namespace std ;

namespace xdb {

Transaction::Transaction(Connection &con): con_(con.handle()) {
    con_->begin() ;
}

void Transaction::commit() {
    con_->commit() ;
}

void Transaction::rollback() {
    con_->rollback() ;

}

} // namespace db

