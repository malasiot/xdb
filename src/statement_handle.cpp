#include <wspp/database/statement_handle.hpp>
#include <wspp/database/exception.hpp>

#include <boost/format.hpp>

#include <cassert>
#include <cstring>

using namespace std ;

namespace cvx { namespace db {

/*
StatementHandle::StatementHandle(sqlite3 *con, const string &sql): last_arg_idx_(0)
{
    assert(con) ;

    const char * tail = 0;

    if ( sqlite3_prepare_v2(con, sql.c_str(), -1, &handle_ ,&tail) != SQLITE_OK )
        throw Exception(con) ;


    int num_fields = sqlite3_column_count(handle_);

    for( int index = 0; index < num_fields; index++ ) {
        const char* field_name = sqlite3_column_name(handle_, index);
        field_map_[field_name] = index ;
    }
}*/

//int StatementHandle::columnIdx(const string &name) const {
//    auto it = field_map_.find(name) ;
//    if ( it != field_map_.end() ) return (*it).second ;
 //   else return -1 ;
//}

#if 0


int StatementHandle::columns() const {
    check() ;
    return ( sqlite3_data_count(handle_) ) ;
}

const char *StatementHandle::columnName(int idx) const {
   check() ;
   const char *name = sqlite3_column_name(handle_, idx)  ;
   if ( name == nullptr ) throw Exception(str(boost::format("There is no column with index %d") % idx)) ;
   else return name ;
}

int StatementHandle::columnType(int idx) const {
    check() ;
    return sqlite3_column_type(handle_, idx);
}

int StatementHandle::columnBytes(int idx) const {
    check() ;
    return sqlite3_column_bytes(handle_, idx);
}


template<>
int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int(handle_, idx);
}

template<>
unsigned int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int(handle_, idx);
}

template<>
short int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int(handle_, idx);
}

template<>
unsigned short int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int(handle_, idx);
}

template<>
long int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int64(handle_, idx);
}

template<>
unsigned long int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int64(handle_, idx);
}

template<>
bool StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int(handle_, idx);
}

template<>
double StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_double(handle_, idx);
}

template<>
float StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_double(handle_, idx);
}

template<>
long long int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int64(handle_, idx);
}

template<>
unsigned long long int StatementHandle::get(int idx) const
{
    check() ;
    return sqlite3_column_int64(handle_, idx);
}

template<>
char const* StatementHandle::get(int idx) const
{
    return reinterpret_cast<char const*>(sqlite3_column_text(handle_, idx));
}

template<>
std::string StatementHandle::get(int idx) const
{
    const char *res = reinterpret_cast<char const*>(sqlite3_column_text(handle_, idx));
    if ( res == nullptr ) return string() ;
    else return res ;
}

template<>
Blob StatementHandle::get(int idx) const
{
    check() ;
    const void *data = sqlite3_column_blob(handle_, idx);
    int bytes = sqlite3_column_bytes(handle_, idx) ;
    return Blob((const char *)data, bytes) ;
}

template<>
void StatementHandle::read(int idx, int &val) const
{
    check() ;
    val = sqlite3_column_int(handle_, idx);
}

template<>
void StatementHandle::read(int idx, unsigned int &val) const
{
    check() ;
    val = sqlite3_column_int(handle_, idx);
}

template<>
void StatementHandle::read(int idx, short int &val) const
{
    check() ;
    val = sqlite3_column_int(handle_, idx);
}

template<>
void StatementHandle::read(int idx, unsigned short &val) const
{
    check() ;
    val = sqlite3_column_int(handle_, idx);
}

template<>
void StatementHandle::read(int idx, long int &val) const
{
    check() ;
    val = sqlite3_column_int64(handle_, idx);
}

template<>
void StatementHandle::read(int idx, unsigned long &val) const
{
    check() ;
    val = sqlite3_column_int64(handle_, idx);
}

template<>
void StatementHandle::read(int idx, bool &v) const
{
    check() ;
    v = sqlite3_column_int(handle_, idx);
}

template<>
void StatementHandle::read(int idx, double &val) const
{
    check() ;
    val = sqlite3_column_double(handle_, idx);
}

template<>
void StatementHandle::read(int idx, float &val) const
{
    check() ;
    val = sqlite3_column_double(handle_, idx);
}

template<>
void StatementHandle::read(int idx, long long int &val) const
{
    check() ;
    val = sqlite3_column_int64(handle_, idx);
}

template<>
void StatementHandle::read(int idx, unsigned long long &val) const
{
    check() ;
    val = sqlite3_column_int64(handle_, idx);
}


template<>
void StatementHandle::read(int idx, std::string &val) const
{
    const char *res = reinterpret_cast<char const*>(sqlite3_column_text(handle_, idx));
    if ( res == nullptr ) return  ;
    val.assign(res) ;
}

template<>
void StatementHandle::read(int idx, Blob &blob) const
{
    check() ;
    const void *data = sqlite3_column_blob(handle_, idx);
    int bytes = sqlite3_column_bytes(handle_, idx) ;
    blob = Blob((const char *)data, bytes) ;
}

#endif

} // namespace db
} // namespace wspp
