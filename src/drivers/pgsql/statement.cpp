#include "statement.hpp"
#include "exceptions.hpp"
#include "query_result.hpp"
#include "string_conv.hpp"
#include <cstring>

#include <random>

using namespace std ;


namespace xdb {


void PGSQLStatementHandle::check() const {
    if ( !handle_ )
        throw Exception("Statement has not been compiled.");
}

void PGSQLStatementHandle::checkIdx(int idx) const {
    if ( idx >= values_.size() )
        throw Exception("Bound parameter index out of range.");
}

// https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c

static std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static std::mt19937 rg{std::random_device{}()};
    static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

void PGSQLStatementHandle::prepare()
{
    if ( name_.empty() ) {
        name_ = "ps_" + random_string(8) ;

        PGresult *r = PQprepare(handle_, name_.c_str(), sql_.c_str(), 0, nullptr) ;

        if ( !checkResult(r) ) {
           
            PQclear(r) ;
            throw PGSQLException(handle_) ;
        } 

        PGresult* describe_res = PQdescribePrepared(handle_, name_.c_str());
        int nparams = -1;

        if ( PQresultStatus(describe_res) == PGRES_COMMAND_OK) {
            nparams = PQnparams(describe_res);
        } else {
                throw PGSQLException(handle_);
        }

        PQclear(describe_res);
         
        values_.resize(nparams, nullptr);
        lengths_.resize(nparams, 0);
        strings_.resize(nparams) ;
        formats_.resize(nparams, 0) ;
    }

}

void PGSQLStatementHandle::clear() {
    std::fill(lengths_.begin(), lengths_.end(), 0);
    std::fill(values_.begin(), values_.end(), nullptr);
    std::fill(strings_.begin(), strings_.end(), std::string());
    std::fill(formats_.begin(), formats_.end(), 0);
}

void PGSQLStatementHandle::finalize()
{

}


StatementHandle &PGSQLStatementHandle::bind(int idx, const NullType &v) {
    checkIdx(idx);
    return *this ;
}


template<class T>
static void bind_param(std::vector<std::string> &strings, std::vector<const char *> &values,
    std::vector<int> &lengths, int idx, const T &v) {
    if ( idx >= values.size() )
        throw Exception("Bound parameter index out of range.");
    string s = pq_to_string(v) ;
    strings[idx] = std::move(s) ;
    lengths[idx] = s.size() ;
    values[idx] = strings[idx].data();
} 

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned char v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, char v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}


StatementHandle &PGSQLStatementHandle::bind(int idx, int v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned int v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned short int v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, short int v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, long int v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned long int v) {
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, long long int v){
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, unsigned long long int v){
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, double v){
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, float v){
    check();
    bind_param(strings_, values_, lengths_, idx, v);
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, const string &v){
    if ( idx >= values_.size() )
        throw Exception("Bound parameter index out of range.");
    strings_[idx] = v ;
    lengths_[idx] = v.size() ;
    values_[idx] = strings_[idx].data();
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, const Blob &blob){
    if ( idx >= values_.size() )
        throw Exception("Bound parameter index out of range.");
    lengths_[idx] = blob.size() ;
    values_[idx] = reinterpret_cast<const char *>(blob.data());
    formats_[idx] = 1 ;
    return *this ;
}

StatementHandle &PGSQLStatementHandle::bind(int idx, const char *v){
    if ( idx >= values_.size() )
        throw Exception("Bound parameter index out of range.");
    strings_[idx] = v ;
    lengths_[idx] = strlen(v) ;
    values_[idx] = strings_[idx].data();
    return *this ;
}

int PGSQLStatementHandle::placeholderNameToIndex(const std::string &name) {
    return stoi(name.c_str()) ;

}

void PGSQLStatementHandle::exec()
{
    doExec() ;
}

PGresult *PGSQLStatementHandle::doExec()
{
    PGresult *res ;

    if ( !name_.empty() ) {
        res = PQexecPrepared(handle_, name_.c_str(), int(values_.size()),
                             &values_[0],  &lengths_[0], &formats_[0], 0);
    }
    else {
        res = PQexecParams(handle_, sql_.c_str(), int(values_.size()), nullptr,
                           &values_[0],  &lengths_[0], &formats_[0], 0);
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

} // namespace xdb

