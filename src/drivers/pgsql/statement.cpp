#include "statement.hpp"
#include "exceptions.hpp"
#include "query_result.hpp"

#include <cstring>

#include <wspp/util/crypto.hpp>

using namespace std ;
using namespace wspp::util ;

namespace wspp { namespace db {


void PGSQLStatementHandle::check() const {
    if ( !handle_ )
        throw Exception("Statement has not been compiled.");
}

void PGSQLStatementHandle::prepare()
{
    if ( name_.empty() ) {
        name_ = binToHex(randomBytes(16)) ;

        PGresult *r = PQprepare(handle_, name_.c_str(), sql_.c_str(), 0, nullptr) ;

        if ( !checkResult(r) ) {
            PQclear(r) ;
            throw PGSQLException(handle_) ;
        }
    }

}

void PGSQLStatementHandle::clear() {
    params_.clear() ;
}

void PGSQLStatementHandle::finalize()
{

}


StatementHandle &PGSQLStatementHandle::bind(int idx, const NullType &v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}


StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned char v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, char v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, int v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned int v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned short int v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, short int v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, long int v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned long int v) {
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, long long int v){
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned long long int v){
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, double v){
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, float v){
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, const string &v){
    check();
    params_.add(idx, v) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, const Blob &blob){
    check();
    params_.add(idx, blob) ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, const char *v){
    check();
    params_.add(idx, v) ;
    return *this ;
}

int PGSQLStatementHandle::placeholderNameToIndex(const std::string &name) {
    return boost::lexical_cast<int>(name.c_str()+1) ;
}

void PGSQLStatementHandle::exec()
{
    doExec() ;
}

PGresult *PGSQLStatementHandle::doExec()
{
    prepare() ;

    vector<const char *> values ;
    vector<int> lengths, binaries ;
    params_.marshall(values, lengths, binaries) ;

    PGresult *res ;

    if ( !name_.empty() ) {
        res = PQexecPrepared(handle_, name_.c_str(), int(params_.size()),
                             &values[0],  &lengths[0], &binaries[0], 0);
    }
    else {
        res = PQexecParams(handle_, sql_.c_str(), int(values.size()), nullptr,
                           &values[0],  &lengths[0], &binaries[0], 0);
    }

    if ( !checkResult(res) ) {
        PQclear(res) ;
        throw PGSQLException(handle_) ;
    }

    return res ;
}

bool PGSQLStatementHandle::checkResult(PGresult *r) const
{
    switch ( PQresultStatus(r) )
    {
    case PGRES_EMPTY_QUERY: // The string sent to the backend was empty.
    case PGRES_COMMAND_OK: // Successful completion of a command returning no data
    case PGRES_TUPLES_OK: // The query successfully executed
    case PGRES_COPY_OUT: // Copy Out (from server) data transfer started
    case PGRES_COPY_IN: // Copy In (to server) data transfer started
        return true;
    case PGRES_BAD_RESPONSE: // The server's response was not understood
    case PGRES_NONFATAL_ERROR:
    case PGRES_FATAL_ERROR:
        return false ;

    default:
        return false ;
    }
    return false;
}

QueryResult PGSQLStatementHandle::execQuery()
{

    return QueryResult(QueryResultHandlePtr(new PGSQLQueryResultHandle(doExec()))) ;
}

} // namespace db
               } // namespace wspp
