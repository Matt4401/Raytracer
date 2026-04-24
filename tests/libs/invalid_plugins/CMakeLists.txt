cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

project(test_invalid_libs)

add_subdirectory(missing_entry)
add_subdirectory(invalid_data)
add_subdirectory(missing_name)
