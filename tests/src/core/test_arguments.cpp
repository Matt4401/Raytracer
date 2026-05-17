/*
** EPITECH PROJECT, 2026
** test_arguments
** File description:
** test_arguments
*/

#include <gtest/gtest.h>

#include <exception/ParsingException.hpp>
#include <string>
#include <vector>

#include "Core.hpp"

#ifndef PLUGINS_PATH
#define PLUGINS_PATH "./plugins/"
#endif

#ifndef TEST_CONFIG_FILE_PATH
#define TEST_CONFIG_FILE_PATH "./tests/configs/"
#endif

static std::string CONFIG_PATH = TEST_CONFIG_FILE_PATH;

TEST(CORE_ARGUMENTS, valid_arguments) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}

// ============================
//  Flag Tests
// ============================

TEST(CORE_ARGUMENTS, valid_export_flag_ppm) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-e", "ppm"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}

TEST(CORE_ARGUMENTS, invalid_export_format) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-e",
                                     "invalid_format"};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

TEST(CORE_ARGUMENTS, multiple_export_flags) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-e", "ppm",
                                     "-e", "ppm"};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

TEST(CORE_ARGUMENTS, export_flag_at_end_missing_data) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-e"};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

// ============================
// Configuration File Tests
// ============================

TEST(CORE_ARGUMENTS, no_config_file) {
    raytracer::Core core;
    std::vector<std::string> args = {};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

TEST(CORE_ARGUMENTS, multiple_config_files) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg",
                                     CONFIG_PATH + "valid.cfg"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}

TEST(CORE_ARGUMENTS, config_file_first_then_export) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-e", "ppm"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}

TEST(CORE_ARGUMENTS, export_flag_before_config_file) {
    raytracer::Core core;
    std::vector<std::string> args = {"-e", "ppm", CONFIG_PATH + "valid.cfg"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}

// ============================
// Unknown Flag Tests
// ============================

TEST(CORE_ARGUMENTS, unknown_flag) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-x"};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

TEST(CORE_ARGUMENTS, unknown_flag_with_data) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "--unknown",
                                     "data"};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

// ============================
// Complex Argument Scenarios
// ============================

TEST(CORE_ARGUMENTS, export_with_empty_format_string) {
    raytracer::Core core;
    std::vector<std::string> args = {CONFIG_PATH + "valid.cfg", "-e", ""};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

TEST(CORE_ARGUMENTS, only_export_flag_no_config) {
    raytracer::Core core;
    std::vector<std::string> args = {"-e", "ppm"};

    ASSERT_THROW(core.init(args, PLUGINS_PATH),
                 raytracer::exception::ParsingException);
}

// ============================
// Edge Cases
// ============================

TEST(CORE_ARGUMENTS, flag_at_beginning_with_config_after) {
    raytracer::Core core;
    std::vector<std::string> args = {"-e", "ppm", CONFIG_PATH + "valid.cfg"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}

TEST(CORE_ARGUMENTS, mixed_valid_flags_and_config) {
    raytracer::Core core;
    std::vector<std::string> args = {"-e", "ppm", CONFIG_PATH + "valid.cfg"};

    ASSERT_NO_THROW(core.init(args, PLUGINS_PATH));
}
