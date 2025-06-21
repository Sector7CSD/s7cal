cmake_minimum_required(VERSION 3.31.6)
project(s7cal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
        termcolor
        GIT_REPOSITORY https://github.com/ikalnytskyi/termcolor.git
        GIT_TAG master
)
FetchContent_MakeAvailable(termcolor)

find_package(yaml-cpp REQUIRED)

add_executable(s7cal
        src/main.cpp
        src/calendar.cpp
        src/holidays.cpp
        src/utils.cpp
        src/utils.h
        src/data.h
        src/userconfig.cpp
        src/userconfig.h
)

target_include_directories(s7cal PRIVATE ${termcolor_SOURCE_DIR}/include)

target_link_libraries(s7cal PRIVATE CLI11::CLI11 yaml-cpp)
