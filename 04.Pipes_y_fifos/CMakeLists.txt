cmake_minimum_required(VERSION 3.9)
project(04_Pipes_y_fifos)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/main3.cpp
    )

add_subdirectory("${PROJECT_SOURCE_DIR}/Fifos")
add_subdirectory("${PROJECT_SOURCE_DIR}/Pipes")


add_executable(04_Pipes_y_fifos ${SOURCE} ${HEADERS})