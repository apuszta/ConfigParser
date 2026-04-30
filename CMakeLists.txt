cmake_minimum_required(VERSION 3.20)
project(ConfigParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(ConfigParser
    main.cpp
    src/Config.cpp
    src/ConfigParser.cpp
)

target_include_directories(ConfigParser PRIVATE src)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests tests/ParserTests.cpp src/ConfigParser.cpp src/Config.cpp)
target_link_libraries(unit_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)