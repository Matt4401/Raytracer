cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    PerlinNoisePlugin.cpp
    PerlinNoise.cpp
)

SET (HEADERS
    PerlinNoise.hpp
)

add_library(perlinnoise_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(perlinnoise_plug PRIVATE raytracer_core)

target_include_directories(perlinnoise_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(perlinnoise_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
