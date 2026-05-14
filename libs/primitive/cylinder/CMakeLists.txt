cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
        CylinderPlugin.cpp
        Cylinder.cpp
        LimitedCylinder.cpp
)

SET (HEADERS
        Cylinder.hpp
        LimitedCylinder.hpp
)

add_library(cylinder_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(cylinder_plug PRIVATE raytracer_core)

target_include_directories(cylinder_plug PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(cylinder_plug PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
