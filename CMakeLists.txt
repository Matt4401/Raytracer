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

add_subdirectory(libs)

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

)

set(HEADERS
        src/Core.hpp
        include/exception/Exception.hpp
        include/exception/ParsingException.hpp
        include/exception/CoreException.hpp
        include/exception/PluginException.hpp
)

add_executable(${NAME} src/main.cpp ${SRCS} ${HEADERS})