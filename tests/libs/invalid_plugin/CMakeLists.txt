cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
	TestPlugin.cpp
)

add_library(test_invalid_plug SHARED ${SRCS})

target_include_directories(test_invalid_plug PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(test_invalid_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/plugins/
)
