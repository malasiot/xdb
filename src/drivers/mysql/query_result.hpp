#ifndef XDB_MYSQL_QUERY_RESULT_HANDLE_HPP
#define XDB_MYSQL_QUERY_RESULT_HANDLE_HPP

#include <xdb/query_result_handle.hpp>
#include <xdb/types.hpp>

#include "statement.hpp"

namespace xdb {

class MySQLQueryResultHandle: public QueryResultHandle {
public:
    MySQLQueryResultHandle(const std::shared_ptr<MySQLStatementHandle> &stmt);

    ~MySQLQueryResultHandle() ;

    int at() const override {
        //return pos_ ;
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
    void check_idx(int idx) const ;

    std::shared_ptr<MySQLStatementHandle> stmt_ ;
    MYSQL_RES* meta_res_;
    size_t column_count_ = 0 ;
    bool is_valid_ = false ;

    struct ColumnBuffer {
        enum_field_types type_;
        std::vector<char> buffer_;
        unsigned long actual_length_;
        my_bool is_null_;
        std::string name_ ;
    };
    std::vector<ColumnBuffer> columns_;
    std::vector<MYSQL_BIND> output_binds_;
    std::unordered_map<std::string, int> name_to_index_;
} ;

}


#endif
