cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
    NormalMapPlugin.cpp
    NormalMap.cpp
)

SET (HEADERS
    NormalMap.hpp
)

add_library(normalmap_plug SHARED ${SRCS} ${HEADERS})

if (APPLE)
    find_package(SFML 3 COMPONENTS Graphics REQUIRED)
else ()
    find_package(SFML 2.6 COMPONENTS graphics REQUIRED)
endif ()

if (APPLE)
    target_link_libraries(normalmap_plug PRIVATE
        raytracer_core
        SFML::Graphics
    )
else ()
    target_link_libraries(normalmap_plug PRIVATE
        raytracer_core
        sfml-graphics
    )
endif ()

target_include_directories(normalmap_plug PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(normalmap_plug PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
