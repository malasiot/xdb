cmake_minimum_required(VERSION 3.14)

# Enable CTest framework for the project
include(CTest)

# 1. Fetch GoogleTest from GitHub automatically
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
# For Windows compatibility with runtime overrides
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 2. Define your test executable
add_executable(run_tests test_main.cpp)

# 3. Link against your app/library targets and GTest targets
# (Replace 'my_lib' with the actual target name of your application logic)
# target_link_libraries(run_tests PRIVATE my_lib)
target_link_libraries(run_tests PRIVATE GTest::gtest_main xdb)

# 4. Automatically discover and register all individual TEST() macros
include(GoogleTest)
gtest_discover_tests(run_tests)