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

    Statement(Connection &con, const std::string &sql) ;

    // helper for creating a connection and binding parameters
    template<typename ...Args>
    Statement(Connection& con, const std::string & sql, Args... args): Statement(con, sql) {
        bindAll(args...) ;
    }

    // clear parameter bindings

    void clear() {
        stmt_->clear() ;
    }

    // bind value to placeholder index

    template <class T>
    Statement &bind(int idx, T v) {
        stmt_->bind(idx, v) ;
        return *this ;
    }

    // bind value by to placeholder by name

    template <class T>
    Statement &bind(const std::string &name, const T &p) {
        stmt_->bind(stmt_->placeholderNameToIndex(name), p) ;
        return *this ;
    }

    // bind all values sequentially

    template <typename ... Args>
    Statement &bindAll(Args&& ... args) {
        return bindm((uint)1, args...) ;
    }

    // bind values and execute statement

    template<typename ...Args>
    void operator()(Args&&... args) {
        bindAll(args...) ;
        exec() ;
    }

    void exec() {
        stmt_->exec() ;
    }

protected:


    Statement &bindm(uint idx) {
        return *this ;
    }

    template <typename T>
    Statement &bindm(uint idx, T&& t) {
        return bind(idx, t) ;
    }

    template <typename First, typename ... Args>
    Statement &bindm(uint idx, First f, Args&& ... args) {
        return bind(idx++, f).bindm(idx, args...) ;
    }

protected:

    StatementHandlePtr stmt_ ;
};

} // namespace xdb



#endif
