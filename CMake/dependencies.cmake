find_package(SQLite3 REQUIRED)
find_package(PostgreSQL QUIET)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARIADB REQUIRED IMPORTED_TARGET libmariadb)