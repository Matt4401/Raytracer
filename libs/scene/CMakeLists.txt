cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    ScenePlugin.cpp
    Scene.cpp
)

SET (HEADERS
    Scene.hpp
)

add_library(scene_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(scene_plug PRIVATE raytracer_core)

target_include_directories(scene_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(scene_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)