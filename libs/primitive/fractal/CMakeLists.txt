cmake_minimum_required(VERSION 3.10)
SET (CMAKE_CXX_STANDARD 20)

SET (SRCS
	FractalPlugin.cpp
	FractalBuilder.cpp
	Fractal3D.cpp
	MandelbulbStrategy.cpp
)

SET (HEADERS
	FractalBuilder.hpp
	Fractal3D.hpp
	IFractalStrategy.hpp
	MandelbulbStrategy.hpp
)

add_library(fractal_plug SHARED ${SRCS} ${HEADERS})
target_link_libraries(fractal_plug PRIVATE raytracer_core)

target_include_directories(fractal_plug PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/libs/primitive
)

set_target_properties(fractal_plug PROPERTIES
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/
)
