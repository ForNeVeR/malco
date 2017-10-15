cmake_minimum_required (VERSION 3.1)
project (malco)

set (CMAKE_CXX_STANDARD 14)
set (PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")
file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.h
        ${PROJECT_SOURCE_DIR}/*.cpp
)
add_executable (malco ${SOURCES})
