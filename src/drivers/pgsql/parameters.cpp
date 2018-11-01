#include "parameters.hpp"

#include <set>
#include <cassert>

using namespace std ;

namespace xdb {

int PreparedStatementParameters::marshall(std::vector<const char *> &values,
        std::vector<int> &lengths,  std::vector<int> &binaries) const
{
    const auto elements = max_idx_;
    const auto array_size = elements + 1;

    // check that we have all entries

    assert(max_idx_ == entries_.size()) ;

    values.clear();
    values.resize(array_size, nullptr);
    lengths.clear();
    lengths.resize(array_size, 0);
    binaries.resize(array_size, 0);

    for( const auto &p: entries_ ) {
        int param = p.first - 1 ;
        const Entry &e = p.second ;

        switch(e.type_) {
        case Type::String:
            values[param] = values_[e.array_idx_].c_str() ;
            lengths[param] = values_[e.array_idx_].size() ;
            break ;
        case Type::Blob:
            values[param] = blobs_[e.array_idx_].data() ;
            lengths[param] = blobs_[e.array_idx_].size() ;
            binaries[param] = 1 ;
            break ;
        case Type::Null:
            break ;
        }

    }

    return elements;
}

} // namespace xdb

