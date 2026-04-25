cmake_minimum_required(VERSION 3.20)
project(ConfigSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ConfigSystem
    main.cpp
    src/Config.cpp
    src/ConfigParser.cpp
)

target_include_directories(ConfigSystem PRIVATE src)
