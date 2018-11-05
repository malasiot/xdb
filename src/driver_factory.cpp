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

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

static bool parse_key_val(std::string &tok, map<string, string> &params) {

    size_t pos = tok.find('=') ;

    string key, val ;

    if ( pos == string::npos ) {
        key = trim(tok) ;
    }
    else {
        key = tok.substr(0, pos) ; trim(key) ;
        val = tok.substr(pos+1) ;  trim(val) ;
    }

    if ( key.empty() ) return false ;
    params.emplace(key, val) ;

    return true ;

}

bool DriverFactory::parseParamString(const string &str, Dictionary &params)
{
    auto previous = str.begin() ;
    auto next = std::find(previous, str.end(), ';') ;

    while ( next != str.end() ) {

        string tok(previous, next) ;
        next = previous + 1;

        if ( ! parse_key_val(tok, params) ) return false ;

        next = std::find(previous, str.end(), ';');
    }

    if ( previous != str.end() )  {
        string tok(previous, str.end()) ;
        if ( !parse_key_val(tok, params) ) return false ;
    }

    return true ;
}

}

