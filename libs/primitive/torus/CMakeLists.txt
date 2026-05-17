cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    TorusPlugin.cpp
    Torus.cpp
)

SET (HEADERS
    Torus.hpp
)

add_library(torus_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(torus_plug PRIVATE raytracer_core)

target_include_directories(torus_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(torus_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)