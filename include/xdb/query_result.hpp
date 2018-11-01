#ifndef XDB_QUERY_RESULT_HPP
#define XDB_QUERY_RESULT_HPP

#include <string>

#include <xdb/dictionary.hpp>
#include <xdb/query_result_handle.hpp>

#include <iostream>

namespace xdb {

class Row ;
class Query ;
class Column ;

class QueryResult
{

public:

    QueryResult(QueryResult &&other) = default ;
    QueryResult(QueryResult &other) = delete ;
    QueryResult& operator=(const QueryResult &other) = delete;
    QueryResult& operator=(QueryResult &&other) = default;

    // read next row
    bool next() {
        return handle_->next() ;
    }

    // number of columns returned
    int columns() const {
        return handle_->columns() ;
    }

    // type of the column

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

    // bytes of this column (blobs)
    int columnBytes(int idx) const ;
    // has a column with given name
    bool hasColumn(const std::string &name) const ;

    template<class T>
    T get(int idx) const {
        T v ;
        handle_->read(idx, v) ;
        return v ;
    }

    template <class T>
    T get(const std::string &name) const {
        int idx = columnIdx(name) ;
        return get<T>(idx) ;
    }

    // advanve cursor and get the row pointed by
    Row getOne() ;

    template<class T>
    void read(int idx, T &val) const {
        handle_->read(idx, val) ;
    }
#ifdef HAS_OPTIONAL
    template<class T>
    void read(int idx, boost::optional<T> &val) const {
        if ( !columnIsNull(idx) ) {
            T v ;
            handle_->read(idx, v) ;
            val = v ;
        }
    }
#endif
    void reset() {
        handle_->reset() ;
    }

    template <typename ... Args>
    void into(Args &... args) const {
        readi(0, args...) ;
    }


    Dictionary getAll() const ;

    class iterator {
    public:
        iterator(QueryResult &res, bool at_end);
        iterator(const iterator &other) = delete;
        iterator(iterator &&other) = default;

        iterator& operator=(const iterator &other) = delete;
        iterator& operator=(iterator &&other) = default;

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

class Row {
public:
    Row(QueryResult &qr): qres_(qr) {}


    Column operator [] (int idx) const { return Column(qres_, idx); }
    Column operator [] (const std::string &name) const { return Column(qres_, name); }


    // number of columns returned
    int columns() const { return qres_.columns(); }
    // type of the column
    int columnType(int idx) const { return qres_.columnType(idx); }
    // name of the column
    std::string columnName(int idx) const { return qres_.columnName(idx); }
    // index of column with given name
    int columnIdx(const std::string &name) const { return qres_.columnIdx(name) ; }
    // bytes of this column (blobs)
    int columnBytes(int idx) const { return qres_.columnBytes(idx); }
    // has a column with given name
    bool hasColumn(const std::string &name) const { return qres_.hasColumn(name); }

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
    friend ColumnAccessor operator >> ( const Row &row, T &val ) ;

private:

    QueryResult &qres_ ;
};

inline Row QueryResult::getOne()  {
        next() ;
        return Row(*this) ;
}

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
