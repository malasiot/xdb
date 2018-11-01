#include "driver_factory.hpp"

#include "drivers/sqlite/driver.hpp"

#ifdef HAS_PGSQL_DRIVER
#include "drivers/pgsql/driver.hpp"
#endif


using namespace std ;

namespace xdb {

std::shared_ptr<ConnectionHandle> DriverFactory::createConnection(const std::string &dsn) const
{
    int pos = dsn.find_first_of(':') ;
    if ( pos == string::npos ) return nullptr ;

    string driver_name = dsn.substr(0, pos) ;
    string param_str = dsn.substr(pos+1) ;

    Dictionary params ;
    parseParamString(param_str, params) ;

    if ( driver_name == "sqlite" )
        return SQLiteDriver::instance().open(params) ;
#ifdef HAS_PGSQL_DRIVER
    if ( driver_name == "pgsql")
        return PGSQLDriver::instance().open(params) ;
#endif
    return nullptr ;

}


bool DriverFactory::parseParamString(const string &str, Dictionary &params)
{
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    boost::char_separator<char> sep(";");

    tokenizer tokens(str, sep);

    for ( auto &&tok: tokens ) {
        size_t pos = tok.find('=') ;

        string key, val ;

        if ( pos == string::npos ) {
            key = boost::trim_copy(tok) ;
        }
        else {
            key = boost::trim_copy(tok.substr(0, pos)) ;
            val = boost::trim_copy(tok.substr(pos+1)) ;
        }

        if ( key.empty() ) return false ;
        params.add(key, val) ;
    }
}

}

