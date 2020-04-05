#include <wspp/database/transaction.hpp>
#include <wspp/database/connection.hpp>

using namespace std ;

namespace cvx { namespace db {

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
} // namespace wspp
