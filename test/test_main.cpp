#include <gtest/gtest.h>
#include <sstream>
#include <xdb/connection.hpp>
#include <optional>

using namespace xdb ;
using namespace std ;

TEST(DbTests, QueryStatement) {
    try {
        Connection con ;
        con.open("sqlite:db=:memory:;mode=rw") ;
        con.execute("CREATE TABLE users (name TEXT, surname TEXT, address TEXT)");
        con.execute("INSERT INTO users (name, surname, address) VALUES ('John', 'Smith', 'A'), ('Mary', 'Stewart', 'B'), ('John', 'Snow', null)") ;

        auto q1 = con.query("SELECT surname, address FROM users WHERE name = ?", "John") ;

        stringstream res ;
        for ( auto row: q1 ) {
            auto surname = row.get<string>("surname") ;
            auto address = row.getOptional<string>(1) ;
            res << surname << ' ' << address.value_or("null") << ' ';
        }

        EXPECT_EQ(res.str(), "Smith A Snow null ");

        res.str("") ;

        auto q2 = con.query("SELECT surname, address FROM users WHERE name = ? LIMIT 1", "Mary") ;
        auto row = q2.getOne() ;
        if ( row ) {
            string surname = row.get<string>("surname") ;
            std::optional<string> address ;

            row >> surname >> address ;
            res << surname << ' ' << address.value_or("null") ;
        }
        EXPECT_EQ(res.str(), "Stewart B");
    } catch ( Exception &e ) {
        cout << e.what() << endl ;
    // EXPECT_EQ(add(2, 3), 5);
    }
}
#if 0
TEST(DbTests, MySQL) {
    try {
        Connection con ;
        con.open("mysql:db=hp;host=localhost;username=malasiot;password=sotiris99") ;
      //  con.execute("CREATE TABLE IF NOT EXISTS users (name TEXT, surname TEXT, address TEXT)");
     //   con.execute("INSERT INTO users (name, surname, address) VALUES ('John', 'Smith', 'A'), ('Mary', 'Stewart', 'B'), ('John', 'Snow', null)") ;

        Blob data ; 
        for ( uint i=0 ; i<100000 ; i++ ) data.push_back((uint8_t)(i%255));

        con.execute("UPDATE users SET bio = ? WHERE id = 1", data) ;

        auto q1 = con.query("SELECT surname, address, bio FROM users WHERE name = ?", "John") ;

        stringstream res ;
        for ( auto row: q1 ) {
            auto surname = row.get<string>("surname") ;
            auto address = row.getOptional<string>(1) ;
            auto bio = row.get<Blob>(2);
            res << surname << ' ' << address.value_or("null") << ' ';
        }

        EXPECT_EQ(res.str(), "Smith A Snow null ");

        res.str("") ;

        auto q2 = con.query("SELECT surname, tel FROM users WHERE name = ? and tel = ? LIMIT 1", "Mary", 345) ;
        auto row = q2.getOne() ;
        if ( row ) {
            string surname = row.get<string>("surname") ;
            std::optional<long long> tel ;

            row >> surname >> tel ;
            res << surname << ' ' << tel.value_or(0) ;
        }
        EXPECT_EQ(res.str(), "Stewart 345");
    } catch ( Exception &e ) {
        cout << e.what() << endl ;
    // EXPECT_EQ(add(2, 3), 5);
    }
}
#endif

TEST(DbTests, PostgreSQL) {
    try {
        Connection con ;
        con.open("pgsql:dbname=hp;host=localhost;port=5432;user=postgres") ;
        con.execute("CREATE TABLE IF NOT EXISTS users (id SERIAL PRIMARY KEY,  name TEXT, surname TEXT, address TEXT, tel INTEGER, bio bytea)");
        con.execute("INSERT INTO users (name, surname, address, tel) VALUES ('John', 'Smith', 'A', 123), ('Mary', 'Stewart', 'B', 345), ('John', 'Snow', null, null)") ;

        Blob data ; 
        for ( uint i=0 ; i<100000 ; i++ ) data.push_back((uint8_t)(i%255));

        con.execute("UPDATE users SET bio = $1 WHERE id = 1", data) ;

        auto q1 = con.query("SELECT surname, address, bio FROM users WHERE name = $1 ORDER by id", "John") ;

        stringstream res ;
        for ( auto row: q1 ) {
            auto surname = row.get<string>("surname") ;
            auto address = row.getOptional<string>(1) ;
            auto bio = row.get<Blob>(2);
            res << surname << ' ' << address.value_or("null") << ' ';
        }

        EXPECT_EQ(res.str(), "Smith A Snow null ");

        res.str("") ;

        auto q2 = con.query("SELECT surname, tel FROM users WHERE name = $1 and tel = $2 LIMIT 1", "Mary", 345) ;
        auto row = q2.getOne() ;
        if ( row ) {
            string surname = row.get<string>("surname") ;
            std::optional<long long> tel ;

            row >> surname >> tel ;
            res << surname << ' ' << tel.value_or(0) ;
        }
        EXPECT_EQ(res.str(), "Stewart 345");
    } catch ( Exception &e ) {
        cout << e.what() << endl ;
    // EXPECT_EQ(add(2, 3), 5);
    }
}