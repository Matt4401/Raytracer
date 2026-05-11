cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    MarblePlugin.cpp
    Marble.cpp
)

SET (HEADERS
    Marble.hpp
)

add_library(marble_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(marble_plug PRIVATE raytracer_core)

target_include_directories(marble_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(marble_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
