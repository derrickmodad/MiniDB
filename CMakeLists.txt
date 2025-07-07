cmake_minimum_required(VERSION 3.31)
project(MiniDB)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES
        src/main.cpp
        src/database.cpp
        src/table.cpp
        src/record.cpp
)

add_executable(MiniDB ${SOURCES})
