cmake_policy(SET CMP0074 NEW)
if(APPLE)
 list(APPEND CMAKE_PREFIX_PATH 
        "/opt/homebrew/opt/sqlite"     # Apple Silicon (M1/M2/M3/M4)
        "/usr/local/opt/sqlite"         # Intel Macs
    )
<<<<<<< HEAD
    SET(PostgreSQL_ADDITIONAL_VERSIONS 17 18)
=======
    set(PostegreSQL_ROOT /opt/homebrew/lib/)
>>>>>>> 9ee52cda7ce44a325ec1190d3d2e98d62c9d6c65
endif()

find_package(SQLite3 REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB QUIET IMPORTED_TARGET libmariadb)