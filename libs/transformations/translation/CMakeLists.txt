cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    Translation.cpp
    TranslationPlugin.cpp
)

SET (HEADERS
    Translation.hpp
)

add_library(translation_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(translation_plug PRIVATE raytracer_core)

target_include_directories(translation_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(translation_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
