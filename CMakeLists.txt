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

set(SRCS
    src/Core.cpp
)

set(HEADERS
    src/Core.hpp
)

add_executable(${NAME} src/main.cpp ${SRCS} ${HEADERS})