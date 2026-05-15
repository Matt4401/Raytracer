cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    Shear.cpp
    ShearPlugin.cpp
)

SET (HEADERS
    Shear.hpp
)

add_library(shear SHARED ${SRCS} ${HEADERS})
target_link_libraries(shear PRIVATE raytracer_core)

target_include_directories(shear PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(shear PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
