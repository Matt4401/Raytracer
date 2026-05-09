cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    MagmaPlugin.cpp
    Magma.cpp
)

SET (HEADERS
    Magma.hpp
)

add_library(magma_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(magma_plug PRIVATE raytracer_core)

target_include_directories(magma_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(magma_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
