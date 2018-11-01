#ifndef XDB_PGSQL_PARAMETERS_HPP
#define XDB_PGSQL_PARAMETERS_HPP

#include <xdb/types.hpp>
#include <vector>
#include <map>

#include "string_conv.hpp"

namespace xdb {

// modified from pqxx

class PreparedStatementParameters {
public:

    PreparedStatementParameters() = default;
    PreparedStatementParameters & operator = (const PreparedStatementParameters &) = delete;

    template<typename T>
    void add(int idx, const T &v) {
        std::string s = pq_to_string(v) ;
        auto res = entries_.insert({idx, Entry{Type::String, (int)values_.size()}}) ;
        if ( res.second ) {
            values_.emplace_back(s) ;
            max_idx_ = std::max(max_idx_,  idx) ;
        }
    }

    void add(int idx, const Blob &b) {

        auto res = entries_.insert({idx, Entry{Type::Blob, (int)blobs_.size()}}) ;
        if ( res.second ) {
            blobs_.emplace_back(b) ;
            max_idx_ = std::max(max_idx_,  idx) ;
        }
    }

    void add(int idx, const NullType &n) {
        auto res = entries_.insert({idx, Entry{Type::Null, 0}}) ;
        if ( res.second )
            max_idx_ = std::max(max_idx_,  idx) ;
    }

    int marshall(
            std::vector<const char *> &values,
            std::vector<int> &lengths,
            std::vector<int> &binaries) const;

    int size() const { return entries_.size() ; }

    void clear() {
        entries_.clear() ;
        values_.clear() ;
        blobs_.clear() ;
        max_idx_ = 0 ;
    }

private:

    enum class Type { Blob, Null, String } ;

    struct Entry {
        Type type_ ;
        int array_idx_ ;
    };

    std::vector<std::string> values_;
    std::vector<Blob> blobs_;
    std::map<int, Entry> entries_ ;
    int max_idx_ = 0 ;

};


} // namespace xdb

#endif
