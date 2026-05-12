cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    MeshPlugin.cpp
    Mesh.cpp
    Face.cpp
    ObjLoader.cpp
    MeshSurfaceHelper.cpp
)

SET (HEADERS
    Mesh.hpp
    Face.hpp
    ObjLoader.hpp
    MeshSurfaceHelper.hpp
)

add_library(mesh_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(mesh_plug PRIVATE raytracer_core)

target_include_directories(mesh_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(mesh_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)