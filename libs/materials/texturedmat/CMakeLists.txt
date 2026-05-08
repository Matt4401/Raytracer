cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    TexturedMaterialPlugin.cpp
    TexturedMaterial.cpp
    MtlLoader.cpp
)

SET (HEADERS
    TexturedMaterial.hpp
    MtlLoader.hpp
    MeshMaterial.hpp
    MeshMaterial.hpp
)

add_library(texturedmat_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(texturedmat_plug PRIVATE raytracer_core)

target_include_directories(texturedmat_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(texturedmat_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
