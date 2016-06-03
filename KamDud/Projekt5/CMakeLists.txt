cmake_minimum_required(VERSION 3.5)
project(Projekt5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES
        main.c)

add_executable(Projekt5 ${SOURCE_FILES})