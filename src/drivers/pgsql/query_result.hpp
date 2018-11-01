#ifndef __PGSQL_QUERY_RESULT_HANDLE_HPP__
#define __PGSQL_QUERY_RESULT_HANDLE_HPP__

#include <wspp/database/query_result_handle.hpp>
#include <wspp/database/types.hpp>

#include "statement.hpp"

#include <memory>

namespace wspp {
namespace db {

using PGResultPtr = std::unique_ptr<PGresult, decltype(&PQclear)> ;

class PGSQLQueryResultHandle: public QueryResultHandle {
public:
    PGSQLQueryResultHandle(PGresult *r);

    ~PGSQLQueryResultHandle() {}

    int at() const override {
        return row_ ;
    }

    bool next() override ;

    int columns() const override ;

    int columnType(int idx) const override ;

    std::string columnName(int idx) const override ;

    int columnIndex(const std::string &name) const override ;

    bool columnIsNull(int idx) const override ;

    void read(int idx, int &val) const override ;
    void read(int idx, unsigned int &val) const override ;
    void read(int idx, short int &val) const override ;
    void read(int idx, unsigned short int &val) const override ;
    void read(int idx, long int &val) const override ;
    void read(int idx, unsigned long int &val) const override ;
    void read(int idx, bool &val) const override ;
    void read(int idx, double &val) const override ;
    void read(int idx, float &val) const override ;
    void read(int idx, long long int &val) const override ;
    void read(int idx, unsigned long long int &val) const override ;
    void read(int idx, std::string &val) const override ;
    void read(int idx, Blob &val) const override ;

    void reset() override;
private:

    void check_has_row() const ;

    PGResultPtr result_ ;
    int row_ = -1, num_rows_ = 0 ;
} ;

}
}

#endif
