cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

project(test_libs)

add_subdirectory(valid_plugin)
add_subdirectory(invalid_plugins)
