SET( SOURCES

    
    connection.cpp
    driver_factory.cpp
    exception.cpp
    statement.cpp
    transaction.cpp
    query.cpp
    query_result.cpp
    statement_handle.cpp

    drivers/sqlite/driver.cpp
    drivers/sqlite/connection.cpp
    drivers/sqlite/exceptions.cpp
    drivers/sqlite/statement.cpp
    drivers/sqlite/query_result.cpp
)

IF ( PostgreSQL_FOUND )
    ADD_DEFINITIONS("-DHAS_PGSQL_DRIVER")
    LIST(APPEND SOURCES
        drivers/pgsql/driver.cpp
        drivers/pgsql/connection.cpp
        drivers/pgsql/exceptions.cpp
        drivers/pgsql/statement.cpp
        drivers/pgsql/query_result.cpp
        drivers/pgsql/parameters.cpp
)
ENDIF ( PostgreSQL_FOUND )

ADD_LIBRARY(xdb SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(xdb ${SQLITE3_LIBRARY} ${PostgreSQL_LIBRARIES} dl z pthread )


