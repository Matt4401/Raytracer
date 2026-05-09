cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    RustPlugin.cpp
    Rust.cpp
)

SET (HEADERS
    Rust.hpp
)

add_library(rust_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(rust_plug PRIVATE raytracer_core)

target_include_directories(rust_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(rust_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
