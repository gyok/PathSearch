cmake_minimum_required(VERSION 3.5)

project(PathSearch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(PathSearcher STATIC "PathSearch.cpp" "PathSearch.h")

add_executable(PathSearch main.cpp)
target_link_libraries(PathSearch PathSearcher)
