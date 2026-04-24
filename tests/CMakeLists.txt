set(FIXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
set(TEST_PLUGINS_DIR "${CMAKE_SOURCE_DIR}/tests/plugins/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

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
else ()
    message(WARNING "GoogleTest not available. Unit tests will not be built.")
endif ()

add_subdirectory(libs)
