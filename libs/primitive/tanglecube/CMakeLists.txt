cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    TangleCubePlugin.cpp
    TangleCube.cpp
)

SET (HEADERS
    TangleCube.hpp
)

add_library(tanglechube_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(tanglechube_plug PRIVATE raytracer_core)

target_include_directories(tanglechube_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(tanglechube_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
