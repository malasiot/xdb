#include <xdb/query_result.hpp>

using namespace std ;

namespace xdb {

QueryResult::Dictionary QueryResult::getAll() const
{
    Dictionary res ;
    for( int i=0 ; i<columns() ; i++ ) {
        res.emplace(columnName(i), get<string>(i) ) ;
    }
    return res ;
}


QueryResult::iterator::iterator(QueryResult &res, bool at_end): qres_(res), at_end_(at_end), current_(new Row(qres_)) {}

} // namespace xdb

