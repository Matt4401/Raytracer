cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    DirectionalLightPlugin.cpp
    DirectionalLight.cpp
)

SET (HEADERS
    DirectionalLight.hpp
)

add_library(directional_light_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(directional_light_plug PRIVATE raytracer_core)

target_include_directories(directional_light_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(directional_light_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)