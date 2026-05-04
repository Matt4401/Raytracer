cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    TrianglePlugin.cpp
    Triangle.cpp
)

SET (HEADERS
    Triangle.hpp
)

add_library(triangle_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(triangle_plug PRIVATE raytracer_core)

target_include_directories(triangle_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(triangle_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)