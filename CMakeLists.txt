cmake_minimum_required (VERSION 3.1)
project (malco)

set (CMAKE_CXX_STANDARD 14)
set (PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")
add_executable (malco "${PROJECT_SOURCE_DIR}/main.cpp")
