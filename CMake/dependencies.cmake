if(APPLE)
 list(APPEND CMAKE_PREFIX_PATH 
        "/opt/homebrew/opt/sqlite"     # Apple Silicon (M1/M2/M3/M4)
        "/usr/local/opt/sqlite"         # Intel Macs
    )
endif()

find_package(SQLite3 REQUIRED)
message(${SQLite3_INCLUDE_DIR})
find_package(PostgreSQL QUIET)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB REQUIRED IMPORTED_TARGET libmariadb)