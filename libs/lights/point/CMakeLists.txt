cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    PointLightPlugin.cpp
    PointLight.cpp
)

SET (HEADERS
    PointLight.hpp
)

add_library(point_light_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(point_light_plug PRIVATE raytracer_core)

target_include_directories(point_light_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(point_light_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)