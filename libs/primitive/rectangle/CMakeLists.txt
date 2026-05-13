cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    RectanglePlugin.cpp
    Rectangle.cpp
)

SET (HEADERS
    Rectangle.hpp
)

add_library(rectangle_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(rectangle_plug PRIVATE raytracer_core)

target_include_directories(rectangle_plug PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(rectangle_plug PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)