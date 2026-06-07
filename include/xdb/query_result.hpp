#ifndef XDB_QUERY_RESULT_HPP
#define XDB_QUERY_RESULT_HPP

#include <string>
#include <map>

#include <xdb/query_result_handle.hpp>

#include <iostream>

namespace xdb {

class Row ;
class Query ;
class Column ;

// stores the result of a query operation. you may then iterate over the results with two ways
//  while ( res.next() ) {
//      string val = res.get<string>(0), address ;
//      res.into(val, address) ;
//  } 
// or using a for loop
//  for ( auto row: res ) {
//      string val = row.get<string>(0), address ;
//      row.int(val, address) ;
//      row >> val >> address ;
//  }

class QueryResult
{
    using dictionary_t = std::map<std::string, std::string> ;
public:

    QueryResult(QueryResult &&other) = default ;
    QueryResult(QueryResult &other) = delete ;
    QueryResult& operator=(const QueryResult &other) = delete;
    QueryResult& operator=(QueryResult &&other) = default;

    // read next row return false if finished
    bool next() {
        is_valid_ = handle_->next() ;
        return is_valid_ ;
    }

    // number of columns returned
    int columns() const {
        return handle_->columns() ;
    }

    // type of the column (driver dependent)
    int columnType(int idx) const {
        return handle_->columnType(idx) ;
    }

    // name of the column
    std::string columnName(int idx) const {
        return handle_->columnName(idx) ;
    }

    // index of column with given name
    int columnIdx(const std::string &name) const {
        return handle_->columnIndex(name) ;
    }

    bool columnIsNull(int idx) const {
        return handle_->columnIsNull(idx) ;
    }

    operator bool () const { return is_valid_ ; }

    // get result column (indexes are zero based)
    template<class T>
    T get(int idx) const {
        T v ;
        handle_->read(idx, v) ;
        return v ;
    }

    // support for nullable columns
    template<class T>
    std::optional<T> getOptional(int idx) const {
        std::optional<T> v ;
        read(idx, v) ;
        return v ;
    }

    // get column by name
    template <class T>
    T get(const std::string &name) const {
        int idx = columnIdx(name) ;
        return get<T>(idx) ;
    }

    template <class T>
    std::optional<T> getOptional(const std::string &name) const {
        int idx = columnIdx(name) ;
        return getOptional<T>(idx) ;
    }

    // advanve cursor and get the row pointed by
    Row getOne() ;

    // read is same with get but allows template parameter deduction
    template<class T>
    void read(int idx, T &val) const {
        handle_->read(idx, val) ;
    }

    template<class T>
    void read(int idx, std::optional<T> &val) const {
        if ( !columnIsNull(idx) ) {
            T v ;
            handle_->read(idx, v) ;
            val = v ;
        }
    }

    // go to the first result
    void reset() {
        handle_->reset() ;
    }

    // helper to read all columns into variables
    template <typename ... Args>
    void into(Args &... args) const {
        readi(0, args...) ;
    }

    dictionary_t getAll() const ;

    // iterator that allows to use a loop of the form 
    // for( auto row: result ) { ... }
    class iterator {
    public:
        iterator(QueryResult &res, bool at_end);
        iterator(const iterator &other) = delete;
        iterator(iterator &&other) = default;

        iterator& operator=(const iterator &other) = delete;
        iterator& operator=(iterator &&other) = delete;

        bool operator==(const iterator &other) const { return ( at_end_ == other.at_end_ ) ; }
        bool operator!=(const iterator &other) const { return ( at_end_ != other.at_end_ ) ; }

        iterator& operator++() {
            at_end_ = !qres_.next() ;
            return *this;
        }

        const Row& operator*() const { return *current_; }

    private:
        QueryResult &qres_ ;
        bool at_end_ ;
        std::unique_ptr<Row> current_ ;

    };

    iterator begin() { return iterator(*this, !next()) ; }
    iterator end() { return iterator(*this, true) ; }

    QueryResult(QueryResultHandlePtr handle): handle_(handle) {}

private:

    QueryResultHandlePtr handle_ ;
    bool is_valid_ = false ;

    template <typename T>
    void readi(int idx, T &t) const {
        read(idx, t) ;
    }

    template <typename First, typename ... Args>
    void readi(int idx, First &f, Args &... args) const {
        readi(idx, f) ;
        readi(idx+1, args...) ;
    }

};

// helper class 
class Column {
public:

    template <class T> T as() const {
        return qres_.get<T>(idx_) ;
    }

private:

    friend class Row ;
    friend class QueryResult ;

    Column(QueryResult &qr, int idx): qres_(qr), idx_(idx) {}
    Column(QueryResult &qr, const std::string &name): qres_(qr), idx_(qr.columnIdx(name)) {}

    QueryResult &qres_ ;
    int idx_ ;
};


class ColumnAccessor ;
// This works together with QueryResult to achieve for loops iteration
class Row {
public:
    Row(QueryResult &qr): qres_(qr) {}


    Column operator [] (int idx) const { return Column(qres_, idx); }
    Column operator [] (const std::string &name) const { return Column(qres_, name); }

    operator bool() const { return (bool)qres_ ; }

    // number of columns returned
    int columns() const { return qres_.columns(); }
    // type of the column
    int columnType(int idx) const { return qres_.columnType(idx); }
    // name of the column
    std::string columnName(int idx) const { return qres_.columnName(idx); }
    // index of column with given name
    int columnIdx(const std::string &name) const { return qres_.columnIdx(name) ; }
    
    template <typename ... Args>
    void into(Args &... args) const {
        qres_.into(args...) ;
    }

    template<class T>
    void read(int idx, T &val) const {
        qres_.read(idx, val) ;
    }

    template<class T>
    T get(int idx) const {
        return qres_.get<T>(idx) ;
    }

    template <class T>
    T get(const std::string &name) const {
        return qres_.get<T>(name) ;
    }

    template<class T>
    std::optional<T> getOptional(int idx) const {
        return qres_.getOptional<T>(idx) ;
    }

    template <class T>
    std::optional<T> getOptional(const std::string &name) const {
        return qres_.get<T>(name) ;
    }

    template<class T>
    friend ColumnAccessor operator >> ( const Row &row, T &val ) ;

private:

    QueryResult &qres_ ;
};

inline Row QueryResult::getOne()  {
        next() ;
        return Row(*this) ;
}

// Helper class used to make the >> operator
class ColumnAccessor {
public:

    ColumnAccessor(const Row &row): row_(row), idx_(0) {}

    template<class T>
    ColumnAccessor &operator >> (T &v) {
        row_.read(++idx_, v) ;
        return *this ;
    }

    const Row &row_ ;
    int idx_ ;
};


template<class T>
ColumnAccessor operator >> ( const Row &row, T &val ) {
    row.read(0, val) ;
    return ColumnAccessor(row) ;
}


} // namespace xdb


#endif
