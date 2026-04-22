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

# libs

# Used to set the path of .so set_target_properties( ${LibNAME1} ${LibName2}
# PROPERTIES LIBRARY_OUTPUT_DIRECTORY
# "${CMAKE_BINARY_DIR}/../plugins/${Category}/")

# Les fonctions du core
set(SOURCES src/core/main.cpp)

# Creation de l'exec arcade
add_executable(${NAME} ${SOURCES})
target_link_libraries(${NAME} PRIVATE -ldl)
