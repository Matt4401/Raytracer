cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
        ConePlugin.cpp
        Cone.cpp
        LimitedCone.cpp
)

SET (HEADERS
        Cone.hpp
        LimitedCone.hpp
)

add_library(cone_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(cone_plug PRIVATE raytracer_core)

target_include_directories(cone_plug PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(cone_plug PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)