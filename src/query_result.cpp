#include <xdb/query_result.hpp>

using namespace std ;

namespace xdb {

Dictionary QueryResult::getAll() const
{
    Dictionary res ;
    for( int i=0 ; i<columns() ; i++ ) {
        res.add(columnName(i), get<string>(i) ) ;
    }
    return res ;
}


QueryResult::iterator::iterator(QueryResult &res, bool at_end): qres_(res), at_end_(at_end), current_(new Row(qres_)) {}

} // namespace xdb

