cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    CrackedDirtPlugin.cpp
    CrackedDirt.cpp
)

SET (HEADERS
    CrackedDirt.hpp
)

add_library(crackeddirt_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(crackeddirt_plug PRIVATE raytracer_core)

target_include_directories(crackeddirt_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(crackeddirt_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
