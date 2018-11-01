#ifndef XDB_QUERY_RESULT_HANDLE_HPP
#define XDB_QUERY_RESULT_HANDLE_HPP

#include <memory>
#include <xdb/types.hpp>

namespace xdb {

// abstract rowset to be implemented by drivers

class QueryResultHandle {
public:
    QueryResultHandle() = default ;

    virtual ~QueryResultHandle() {}

    virtual bool next() = 0 ;

    // number of columns returned
    virtual int columns() const = 0 ;

    virtual int columnType(int idx) const = 0 ;

    virtual std::string columnName(int idx) const = 0 ;

    virtual int columnIndex(const std::string &name) const = 0 ;

    virtual bool columnIsNull(int idx) const = 0 ;

    virtual int at() const = 0;
    virtual void reset() = 0;

    virtual void read(int idx, int &val) const =0 ;
    virtual void read(int idx, unsigned int &val) const =0 ;
    virtual void read(int idx, short int &val) const =0 ;
    virtual void read(int idx, unsigned short int &val) const =0 ;
    virtual void read(int idx, long int &val) const =0 ;
    virtual void read(int idx, unsigned long int &val) const =0 ;
    virtual void read(int idx, bool &val) const =0 ;
    virtual void read(int idx, double &val) const =0 ;
    virtual void read(int idx, float &val) const =0 ;
    virtual void read(int idx, long long int &val) const =0 ;
    virtual void read(int idx, unsigned long long int &val) const =0 ;
    virtual void read(int idx, std::string &val) const =0 ;
    virtual void read(int idx, Blob &val) const =0 ;
} ;

typedef std::shared_ptr<QueryResultHandle> QueryResultHandlePtr ;


}

#endif
