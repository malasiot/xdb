#include <xdb/transaction.hpp>
#include <xdb/connection.hpp>

using namespace std ;

namespace xdb {

Transaction::Transaction(Connection &con): con_(con.handle()), committed_(false) {
    con_->begin() ;
}

Transaction::Transaction(Transaction&& other) noexcept 
        : con_(other.con_), committed_(other.committed_) {
        other.committed_ = true; // Neutralize the moved-from guard
}

Transaction::~Transaction() {
    if ( !committed_ ) {
        try {
            con_->rollback();
        } catch (...) {
              
        }
    }
}

void Transaction::commit() {
    if ( committed_ ) return ;
    con_->commit() ;
    committed_ = true ;
}

void Transaction::rollback() {
    if ( committed_ ) return ;
    con_->rollback() ;
    committed_ = true ;
}

} // namespace db

