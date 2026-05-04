cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    FlatColorPlugin.cpp
    FlatColor.cpp
)

SET (HEADERS
    FlatColor.hpp
)

add_library(flatcolor_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(flatcolor_plug PRIVATE raytracer_core)

target_include_directories(flatcolor_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(flatcolor_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
