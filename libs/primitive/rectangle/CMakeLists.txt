cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

include_directories(../../../include)

SET (SRCS
    RectanglePlugin.cpp
    Rectangle.cpp
)

SET (HEADERS
    Rectangle.hpp
)

add_library(rectangle_plug SHARED ${SRCS} ${HEADERS})
set_target_properties(rectangle_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)