set( SOURCES
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

if ( PostgreSQL_FOUND )
    add_definitions("-DHAS_PGSQL_DRIVER")
    list(APPEND SOURCES
        drivers/pgsql/driver.cpp
        drivers/pgsql/connection.cpp
        drivers/pgsql/exceptions.cpp
        drivers/pgsql/statement.cpp
        drivers/pgsql/query_result.cpp
        drivers/pgsql/parameters.cpp
)
endif ( PostgreSQL_FOUND )

add_library(xdb ${LIBRARY_TYPE} ${SOURCES})

target_include_directories(xdb PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE ${CMAKE_SOURCE_DIR}/src)


include_directories(
   ${SQLite3_INCLUDE_DIRS}
   ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(xdb
    ${SQLite3_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    dl
    z
    pthread
)

# Install library
install(TARGETS xdb
  EXPORT ${PROJECT_NAME}-export
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT stlib
  COMPONENT dev)

# Install headers
install(DIRECTORY "${LIBRARY_INCLUDE_DIR}" DESTINATION "${INSTALL_INCLUDE_DIR}/xdb" )


TARGET_LINK_LIBRARIES(xdb ${SQLITE3_LIBRARY} ${PostgreSQL_LIBRARIES} dl z pthread )


