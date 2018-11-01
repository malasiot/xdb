#include "query_result.hpp"
#include "exceptions.hpp"

#include <wspp/database/types.hpp>

#include <boost/format.hpp>

#include "string_conv.hpp"

using namespace std ;
namespace wspp {
namespace db {


PGSQLQueryResultHandle::PGSQLQueryResultHandle(PGresult *res): result_(res, &PQclear) {

    num_rows_ = PQntuples(res) ;
}

void PGSQLQueryResultHandle::reset() {
    row_ = -1 ;
}

void PGSQLQueryResultHandle::check_has_row() const
{
    if ( row_ < 0 )
        throw Exception("No current row in result") ;
}

bool PGSQLQueryResultHandle::next() {
    ++row_ ;
    if ( row_ >= num_rows_ ) {
        row_ = -2 ;
        return false ;
    }
}

int PGSQLQueryResultHandle::columns() const  {
    return PQnfields(result_.get()) ;
}

int PGSQLQueryResultHandle::columnType(int idx) const {
return PQftype(result_.get(), idx) ;
}

std::string PGSQLQueryResultHandle::columnName(int idx) const  {
    return PQfname(result_.get(), idx) ;
}

int PGSQLQueryResultHandle::columnIndex(const std::string &name) const {
    return PQfnumber(result_.get(), name.c_str()) ;
}

bool PGSQLQueryResultHandle::columnIsNull(int idx) const {
    check_has_row() ;
    return PQgetisnull(result_.get(), row_, idx) ;
}

void PGSQLQueryResultHandle::read(int idx, int &val) const
{
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, unsigned int &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, short int &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, unsigned short &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, long int &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, unsigned long &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, bool &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, double &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, float &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, long long int &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, unsigned long long &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, std::string &val) const {
    check_has_row() ;
    pq_from_string(PQgetvalue(result_.get(), row_, idx), val) ;
}

void PGSQLQueryResultHandle::read(int idx, Blob &blob) const {
    check_has_row() ;
    char *blob_bytes = PQgetvalue(result_.get(), row_, idx) ;
    size_t blob_size = PQgetlength(result_.get(), row_, idx);

    blob = Blob((const char *)blob_bytes, blob_size) ;
}

}
}


