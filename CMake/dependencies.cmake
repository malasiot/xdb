cmake_policy(SET CMP0074 NEW)
if(APPLE)
 list(APPEND CMAKE_PREFIX_PATH 
        "/opt/homebrew/opt/sqlite"     # Apple Silicon (M1/M2/M3/M4)
        "/usr/local/opt/sqlite"         # Intel Macs
    )
    set(PostegreSQL_ROOT /opt/homebrew/lib/)
endif()

find_package(SQLite3 REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB QUIET IMPORTED_TARGET libmariadb)