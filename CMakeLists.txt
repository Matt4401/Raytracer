cmake_minimum_required(VERSION 3.28)

set(NAME "raytracer")

project(
        ${NAME}
        VERSION 1.0.0
        LANGUAGES CXX)

# config CPP
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include)

# Enable testing
enable_testing()
set(BUILD_TESTS ON)

if (BUILD_TESTS)
    add_subdirectory(tests)

    add_custom_target(check
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running all tests"
            VERBATIM
    )
endif ()

set(SRCS
        src/Core.cpp
        src/Render.cpp
        src/exception/Exception.cpp
        src/math/Color.cpp
        src/math/Vector.cpp
        src/math/Rectangle.cpp
        src/math/AABoundingBox.cpp
        src/bvh/BVHNode.cpp
        src/bvh/StrategyFactory.cpp
        src/bvh/ASplitStrategy.cpp
        src/bvh/bvhSplitStrategy/SAHStrategy.cpp
        src/bvh/bvhSplitStrategy/MedianStrategy.cpp
        src/bvh/bvhSplitStrategy/MidpointStrategy.cpp
        src/object/primitive/APrimitive.cpp
        src/util/middleware/Helpers.cpp
        src/plugin/PluginManager.cpp
        src/plugin/ObjectFactory.cpp
        src/object/light/ALight.cpp
        src/object/scene/AScene.cpp
)

set(HEADERS
        src/Core.hpp
        src/Render.hpp
        src/plugin/PluginManager.hpp
        src/plugin/ObjectFactory.hpp
        src/plugin/DlLoader.hpp
)

add_library(raytracer_core STATIC ${SRCS})
set_property(TARGET raytracer_core PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(raytracer_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

add_executable(${NAME} src/main.cpp ${HEADERS})
target_link_libraries(${NAME} PRIVATE raytracer_core)

add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${NAME}>
        ${CMAKE_SOURCE_DIR}/${NAME}
)

add_subdirectory(libs)
