cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    Rotation.cpp
    RotationPlugin.cpp
)

SET (HEADERS
    Rotation.hpp
)

add_library(rotation SHARED ${SRCS} ${HEADERS})
target_link_libraries(rotation PRIVATE raytracer_core)

target_include_directories(rotation PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(rotation PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
