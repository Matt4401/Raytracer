cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    CausticsPlugin.cpp
    Caustics.cpp
)

SET (HEADERS
    Caustics.hpp
)

add_library(caustics_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(caustics_plug PRIVATE raytracer_core)

target_include_directories(caustics_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(caustics_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
