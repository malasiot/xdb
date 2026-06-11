if(APPLE)
 list(APPEND CMAKE_PREFIX_PATH 
        "/opt/homebrew/opt/sqlite"     # Apple Silicon (M1/M2/M3/M4)
        "/usr/local/opt/sqlite"         # Intel Macs
    )
    SET(PostgreSQL_ADDITIONAL_VERSIONS 17 18)
endif()

find_package(SQLite3 REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB REQUIRED IMPORTED_TARGET libmariadb)