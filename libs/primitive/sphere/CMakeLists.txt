cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    SpherePlugin.cpp
    Sphere.cpp
)

SET (HEADERS
    Sphere.hpp
)

add_library(sphere_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(sphere_plug PRIVATE raytracer_core)

target_include_directories(sphere_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(sphere_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)