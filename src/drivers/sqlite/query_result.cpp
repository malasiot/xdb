#include "query_result.hpp"
#include "exceptions.hpp"

#include <wspp/database/types.hpp>

#include <boost/format.hpp>

namespace cvx {
namespace db {


SQLiteQueryResultHandle::SQLiteQueryResultHandle(const std::shared_ptr<cvx::db::SQLiteStatementHandle> &stmt): stmt_(stmt) {

    int num_fields = sqlite3_column_count(stmt_->handle());

    for( int index = 0; index < num_fields; index++ ) {
        const char* field_name = sqlite3_column_name(stmt_->handle(), index);
        column_map_[field_name] = index ;
    }
}

void SQLiteQueryResultHandle::reset() {
    pos_ = -1 ;
    sqlite3_reset(stmt_->handle()) ;
}

void SQLiteQueryResultHandle::check_has_row() const
{
    if ( pos_ < 0 )
        throw Exception("No current row in result") ;
}

bool SQLiteQueryResultHandle::next() {

    if ( pos_ == -2 )
        throw Exception("next called passed the end of the record set");

    switch ( sqlite3_step(stmt_->handle()) ) {
    case SQLITE_ROW:
        pos_ ++ ;
        return true ;
    case SQLITE_DONE:
        pos_ = -2 ;
        return false ;
    default:
        throw SQLiteException(sqlite3_db_handle(stmt_->handle()));
    }
}

int SQLiteQueryResultHandle::columns() const  {
    check_has_row() ;
     return ( sqlite3_data_count(stmt_->handle()) ) ;
}

int SQLiteQueryResultHandle::columnType(int idx) const {
    check_has_row() ;
    return sqlite3_column_type(stmt_->handle(), idx);
}

std::string SQLiteQueryResultHandle::columnName(int idx) const  {
    check_has_row() ;
    const char *name = sqlite3_column_name(stmt_->handle(), idx)  ;
    if ( name == nullptr ) throw Exception(str(boost::format("There is no column with index %d") % idx)) ;
    else return name ;
}

int SQLiteQueryResultHandle::columnIndex(const std::string &name) const {
    check_has_row() ;
    auto it = column_map_.find(name) ;
    if ( it != column_map_.end() ) return (*it).second ;
    else return -1 ;
}

bool SQLiteQueryResultHandle::columnIsNull(int idx) const {
    check_has_row() ;
    return ( sqlite3_column_type(stmt_->handle(), idx) == SQLITE_NULL ) ;
}

void SQLiteQueryResultHandle::read(int idx, int &val) const
{
    check_has_row() ;
    val = sqlite3_column_int(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, unsigned int &val) const {
    check_has_row() ;
    val = sqlite3_column_int(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, short int &val) const {
    check_has_row() ;
    val = sqlite3_column_int(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, unsigned short &val) const {
    check_has_row() ;
    val = sqlite3_column_int(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, long int &val) const {
    check_has_row() ;
    val = sqlite3_column_int64(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, unsigned long &val) const {
    check_has_row() ;
    val = sqlite3_column_int64(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, bool &v) const {
    check_has_row() ;
    v = sqlite3_column_int(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, double &val) const {
    check_has_row() ;
    val = sqlite3_column_double(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, float &val) const {
    check_has_row() ;
    val = sqlite3_column_double(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, long long int &val) const {
    check_has_row() ;
    val = sqlite3_column_int64(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, unsigned long long &val) const {
    check_has_row() ;
    val = sqlite3_column_int64(stmt_->handle(), idx);
}

void SQLiteQueryResultHandle::read(int idx, std::string &val) const {
    check_has_row() ;
    const char *res = reinterpret_cast<char const*>(sqlite3_column_text(stmt_->handle(), idx));
    if ( res == nullptr ) return  ;
    val.assign(res) ;
}

void SQLiteQueryResultHandle::read(int idx, Blob &blob) const {
    check_has_row() ;
    const void *data = sqlite3_column_blob(stmt_->handle(), idx);
    int bytes = sqlite3_column_bytes(stmt_->handle(), idx) ;
    blob = Blob((const char *)data, bytes) ;
}

}
}


