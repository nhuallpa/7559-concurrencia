cmake_minimum_required(VERSION 3.10)
project(SymbolTable)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

add_subdirectory("${PROJECT_SOURCE_DIR}/src/Symbol")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/SymbolTable")

add_executable(SymbolTable ${SOURCE} ${HEADERS})