set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
set(TEST_PLUGINS_DIR "${CMAKE_SOURCE_DIR}/tests/plugins/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(libs)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0)

file(DOWNLOAD "https://api.github.com/zen"
        "${CMAKE_CURRENT_BINARY_DIR}/.github.ping.txt"
        TIMEOUT 5
        STATUS ping_status
)
list(GET ping_status 0 ping_error_code)

if (ping_error_code EQUAL 0)
    set(SKIP_INTERNET FALSE)
else ()
    message(WARNING "No internet connection detected.")
    set(SKIP_INTERNET TRUE)
endif ()

if (NOT SKIP_INTERNET)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    find_package(SFML 3 REQUIRED COMPONENTS Graphics)
    target_link_libraries(exception_tests PRIVATE SFML::Graphics)

    add_executable(exception_tests
            src/main.cpp
            src/exception/exception.cpp
            src/core/test_plugin.cpp
            ${CMAKE_SOURCE_DIR}/src/exception/Exception.cpp
            ${CMAKE_SOURCE_DIR}/src/plugin/ObjectFactory.cpp
            ${CMAKE_SOURCE_DIR}/src/plugin/PluginManager.cpp
    )

    target_link_libraries(exception_tests PRIVATE
            GTest::gtest
            GTest::gmock
            raytracer_core
    )

    target_include_directories(exception_tests PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src
    )

    target_compile_definitions(exception_tests
            PRIVATE
            FIXTURES_PATH="${FIXTURES_DIR}"
            TEST_PLUGINS_PATH="${TEST_PLUGINS_DIR}"
    )

    gtest_discover_tests(exception_tests)

    add_executable(material_tests
            src/main.cpp
            src/materials/test_flatcolor.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/flatcolor/FlatColor.cpp
            src/materials/test_texturedmaterial.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/texturedmat/TexturedMaterial.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/texturedmat/MtlLoader.cpp
            src/materials/test_chessboard.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/chessboard/Chessboard.cpp
            src/materials/test_perlinnoise.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/perlinnoise/PerlinNoise.cpp
            src/materials/test_marble.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/marble/Marble.cpp
            src/materials/test_wood.cpp
            ${CMAKE_SOURCE_DIR}/libs/materials/wood/Wood.cpp
            ${CMAKE_SOURCE_DIR}/src/plugin/ObjectFactory.cpp
            ${CMAKE_SOURCE_DIR}/src/plugin/PluginManager.cpp
    )

    target_link_libraries(material_tests PRIVATE
            GTest::gtest
            GTest::gmock
            raytracer_core
            SFML::Graphics
    )

    target_include_directories(material_tests PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src
    )

    target_compile_definitions(material_tests
            PRIVATE
            FIXTURES_PATH="${FIXTURES_DIR}"
            TEST_PLUGINS_PATH="${TEST_PLUGINS_DIR}"
            PROJECT_PLUGINS_PATH="${CMAKE_SOURCE_DIR}/plugins/"
    )

    gtest_discover_tests(material_tests)

    add_executable(primitive_tests
            src/main.cpp
            src/primitives/test_plane.cpp
            ${CMAKE_SOURCE_DIR}/libs/primitive/plane/Plane.cpp
            src/primitives/test_triangle.cpp
            ${CMAKE_SOURCE_DIR}/src/plugin/ObjectFactory.cpp
            ${CMAKE_SOURCE_DIR}/src/plugin/PluginManager.cpp
    )

    target_link_libraries(primitive_tests PRIVATE
            GTest::gtest
            GTest::gmock
            raytracer_core
    )

    target_include_directories(primitive_tests PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src
    )

    target_compile_definitions(primitive_tests
            PRIVATE
            FIXTURES_PATH="${FIXTURES_DIR}"
            TEST_PLUGINS_PATH="${TEST_PLUGINS_DIR}"
            PROJECT_PLUGINS_PATH="${CMAKE_SOURCE_DIR}/plugins/"
    )

    gtest_discover_tests(primitive_tests)
else ()
    message(WARNING "GoogleTest not available. Unit tests will not be built.")
endif ()

