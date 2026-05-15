cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    Scale.cpp
    ScalePlugin.cpp
)

SET (HEADERS
    Scale.hpp
)

add_library(scale SHARED ${SRCS} ${HEADERS})
target_link_libraries(scale PRIVATE raytracer_core)

target_include_directories(scale PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(scale PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
