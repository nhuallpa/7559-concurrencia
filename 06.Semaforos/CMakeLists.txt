cmake_minimum_required(VERSION 3.10)
project(06_Semaforos)
add_definitions(-DEJEMPLO_1)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/main1.cpp
        )

add_subdirectory("${PROJECT_SOURCE_DIR}/Semaforo")


add_executable(06_Semaforos ${SOURCE} ${HEADERS})