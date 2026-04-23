cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

project(libs)

include_directories(../include)

add_subdirectory(primitive)
add_subdirectory(materials)