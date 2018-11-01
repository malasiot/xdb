#include <xdb/connection.hpp>
#include <xdb/exception.hpp>
#include <xdb/transaction.hpp>

#include <xdb/connection_handle.hpp>

#include "driver_factory.hpp"

#include <fstream>


using namespace std ;

namespace xdb {

Connection::Connection() {
}

Connection::Connection(const std::string &dsn): Connection() {
    open(dsn) ;
}

void Connection::open(const std::string &dsn) {

    if ( dsn.find("uri:file://") == 0 ) {
        string fdsn ;
        ifstream strm(dsn.substr(11)) ;
        std::getline(strm, fdsn) ;
        handle_ = DriverFactory::instance().createConnection(fdsn) ;
    }
    else
        handle_ = DriverFactory::instance().createConnection(dsn) ;

    if ( !handle_ )
        throw Exception("Cannot establish connection with database") ;
}

void Connection::close() {

    handle_->close() ;

}

Statement Connection::prepareStatement(const string &sql)
{
    return Statement(*this, sql) ;
}


Query Connection::prepareQuery(const string &sql) {
    return Query(*this, sql) ;
}

Transaction Connection::transaction() {
    return Transaction(*this) ;
}

void Connection::check() {
    if( !handle_ )
        throw Exception("Database is not open.");
}




} // namespace xdb
