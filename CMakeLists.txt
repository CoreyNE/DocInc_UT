# 1. Standard CMake Setup
cmake_minimum_required(VERSION 3.14)
project(DocInc_Tests)
set(CMAKE_CXX_STANDARD 14)

# 2. Automatically fetch GTest and GMock from Google's repository
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 3. Create the Test Executable
# This tells CMake which files to compile into the final test program
add_executable(
  runTests
  SessionManagerTest.cpp
)

# 4. Link Google Test and Google Mock to your executable
target_link_libraries(
  runTests
  gtest_main
  gmock_main
)

# 5. Enable testing
include(GoogleTest)
gtest_discover_tests(runTests)