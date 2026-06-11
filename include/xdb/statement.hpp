#ifndef XDB_STATEMENT_HPP
#define XDB_STATEMENT_HPP

#include <xdb/exception.hpp>
#include <xdb/statement_handle.hpp>

#include <memory>
#include <string>
#include <utility>

namespace xdb {

class Connection ;

// escape column names/identifiers

std::string escapeName(const std::string &unescaped) ;

class Statement
{
public:

    Statement(Connection &con, const char *sql) ;

    // helper for creating a connection and binding parameters
    template<typename ...Args>
    Statement(Connection& con, const char *sql, Args&&... args): Statement(con, sql) {
        bindAll(std::forward<Args>(args)...) ;
    }

    // clear parameter bindings

    void clear() {
        stmt_->clear() ;
    }

    // bind value to placeholder index

    template <class T>
    Statement &bind(int idx, T &v) {
        stmt_->bind(idx, v);
        return *this;
    }


    template <class T>
    Statement &bind(int idx,  T &&v) {
        stmt_->bind(idx, std::forward<T>(v)) ;
        return *this ;
    }

    // bind value by to placeholder by name

    template <class T>
    Statement &bind(const std::string &name,  T &&p) {
        stmt_->bind(stmt_->placeholderNameToIndex(name), std::forward<T>(p)) ;
        return *this ;
    }

    Statement& bind(int idx, const Blob& v) {
        stmt_->bind(idx, v); 
        return *this;
    }

    Statement& bind(int idx, Blob&& v) = delete;    

    // bind all values sequentially

    template <typename ... Args>
    Statement &bindAll(Args&& ... args) {
        return bindm((uint)0, std::forward<Args>(args)...) ;
    }

    // bind values and execute statement

    template<typename ...Args>
    void operator()(Args&&... args) {
        bindAll(std::forward<Args>(args)...) ;
        exec() ;
    }

    void exec() {
        stmt_->exec() ;
    }

    int64_t execInsert() {
        return stmt_->execInsert() ;
    }

protected:


    Statement &bindm(uint idx) {
        return *this ;
    }

    template <typename T>
    Statement &bindm(uint idx, T&& t) {
        return bind(idx, std::forward<T>(t)) ;
    }

    template <typename T, typename ... Args>
    Statement &bindm(uint idx, T&& f, Args&& ... args) {
        bind(idx, std::forward<T>(f));
        return bindm(idx + 1, std::forward<Args>(args)...);
        return bind(idx++, f).bindm(idx, args...) ;
    }

protected:

    StatementHandlePtr stmt_ ;
};

} // namespace xdb



#endif
