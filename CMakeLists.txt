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
set(RAYTRACER_BUILD_TESTS ON)

if (RAYTRACER_BUILD_TESTS)
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
        src/exporter/ExportPPM.cpp
        src/visual/CliVisual.cpp
        src/visual/SFMLVisual.cpp
        src/visual/SFML/SFMLContext.cpp
        src/visual/SFML/SFMLSelectionPage.cpp
        src/visual/SFML/SFMLPage.cpp
        src/visual/SFML/SFMLImagePage.cpp
        src/exception/Exception.cpp
        src/math/Color.cpp
        src/math/Vector.cpp
        src/math/Matrix4.cpp
        src/math/Noise.cpp
        src/math/Rectangle.cpp
        src/math/AABoundingBox.cpp
		src/math/PolynomialSolver.cpp
        src/bvh/BVHNode.cpp
        src/bvh/StrategyFactory.cpp
        src/bvh/ASplitStrategy.cpp
        src/bvh/bvhSplitStrategy/SAHStrategy.cpp
        src/bvh/bvhSplitStrategy/MedianStrategy.cpp
        src/bvh/bvhSplitStrategy/MidpointStrategy.cpp
        src/object/primitive/APrimitive.cpp
        src/object/transformations/ATransformDecorator.cpp
        src/object/material/ABasicMaterial.cpp
        src/object/material/APerlinBasedMaterial.cpp
        src/object/material/AWorleyBasedMaterial.cpp
        src/util/middleware/Helpers.cpp
        src/plugin/PluginManager.cpp
        src/plugin/ObjectFactory.cpp
        src/parser/ConfigParser.cpp
        src/parser/LibCfgUtils.cpp
        src/object/light/ALight.cpp
        src/object/scene/AScene.cpp
)

set(HEADERS
        src/Core.hpp
        src/Render.hpp
        src/exporter/ExportPPM.hpp
        src/visual/SFML/SFMLImagePage.hpp
        src/visual/SFML/SFMLSelectionPage.hpp
        src/visual/SFML/SFMLPage.hpp
        src/visual/SFML/SFMLContext.hpp
        src/exporter/IExport.hpp
        src/visual/CliVisual.hpp
        src/visual/IVisual.hpp
        src/visual/SFMLVisual.hpp
        src/plugin/PluginManager.hpp
        src/plugin/ObjectFactory.hpp
        src/plugin/DlLoader.hpp
        src/parser/ConfigParser.hpp
        src/parser/LibCfgUtils.hpp
        include/exception/Exception.hpp
        include/exception/ParsingException.hpp
        include/exception/CoreException.hpp
        include/exception/PluginException.hpp
        include/math/Color.hpp
        include/math/Vector.hpp
        include/math/Matrix4.hpp
        include/math/Ray.hpp
		include/math/PolynomialSolver.hpp
        include/object/AObject.hpp
        include/object/ACamera.hpp
        include/object/IObject.hpp
        include/object/ICamera.hpp
        include/object/ILight.hpp
        include/object/IScene.hpp
        include/object/primitive/IPrimitive.hpp
        include/object/primitive/APrimitive.hpp
        include/object/primitive/ReflTypes.hpp
)

# libconfig
include(FetchContent)

FetchContent_Declare(
    libconfig
    GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
    GIT_TAG        v1.7.3
)

# Disable libconfig's own tests/examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS    OFF CACHE BOOL "" FORCE)
# Needed because libconfig 1.7.3 uses cmake_minimum_required < 3.5,
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

FetchContent_MakeAvailable(libconfig)

target_include_directories(config++ SYSTEM INTERFACE
    $<BUILD_INTERFACE:${libconfig_SOURCE_DIR}/lib>
)

target_include_directories(config SYSTEM INTERFACE
    $<BUILD_INTERFACE:${libconfig_SOURCE_DIR}/lib>
)


# core library
add_library(raytracer_core STATIC ${SRCS})
set_property(TARGET raytracer_core PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(raytracer_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

# Link libconfig to the core library (PUBLIC so it propagates to executable)
target_link_libraries(raytracer_core PUBLIC config++)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
target_link_libraries(raytracer_core PRIVATE sfml-graphics sfml-window
                                          sfml-system)

# bin
add_executable(${NAME} src/main.cpp ${HEADERS})
set_target_properties(${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
target_link_libraries(${NAME} PRIVATE raytracer_core)

add_subdirectory(libs)
