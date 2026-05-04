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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

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
        src/exception/Exception.cpp
        src/math/Color.cpp
        src/math/Vector.cpp
        src/math/Rectangle.cpp
        src/object/primitive/APrimitive.cpp
        src/util/middleware/Helpers.cpp
        src/plugin/PluginManager.cpp
        src/plugin/ObjectFactory.cpp
        src/parser/ConfigParser.cpp
        src/object/light/ALight.cpp
)

set(HEADERS
        src/Core.hpp
        src/plugin/PluginManager.hpp
        src/plugin/ObjectFactory.hpp
        src/plugin/DlLoader.hpp
        src/parser/ConfigParser.hpp
        include/exception/Exception.hpp
        include/exception/ParsingException.hpp
        include/exception/CoreException.hpp
        include/exception/PluginException.hpp
        include/math/Color.hpp
        include/math/Vector.hpp
        include/math/Ray.hpp
        include/object/AObject.hpp
        include/object/ACamera.hpp
        include/object/IObject.hpp
        include/object/ICamera.hpp
        include/object/ILight.hpp
        include/object/IScene.hpp
        include/object/primitive/IPrimitive.hpp
        include/object/primitive/APrimitive.hpp
        include/object/primitive/ReflTypes.hpp
        include/util/ObjectMiddleware.hpp
)

add_library(raytracer_core STATIC ${SRCS})
set_target_properties(raytracer_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(raytracer_core PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)


# libconfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

#bin
add_executable(${NAME} src/main.cpp ${HEADERS})
target_link_libraries(${NAME} PRIVATE raytracer_core)
target_link_libraries(${NAME} PRIVATE ${LIBCONFIG_LIBRARIES})
target_include_directories(${NAME} PRIVATE ${LIBCONFIG_INCLUDE_DIRS})

# add_subdirectory(libs)
