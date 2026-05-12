cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    WorleyNoisePlugin.cpp
    WorleyNoise.cpp
)

SET (HEADERS
    WorleyNoise.hpp
)

add_library(worleynoise_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(worleynoise_plug PRIVATE raytracer_core)

target_include_directories(worleynoise_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(worleynoise_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
