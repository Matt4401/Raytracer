/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** exception
*/

#include "exception/Exception.hpp"

#include <gtest/gtest.h>

#include <source_location>
#include <string>

#include "exception/CoreException.hpp"
#include "exception/ParsingException.hpp"
#include "exception/PluginException.hpp"

namespace {

    class ExceptionTest : public testing::Test {
      protected:
        void SetUp() override {
            // Setup for exception tests
        }
    };

}  // namespace

// Tests for base Exception class
TEST_F(ExceptionTest, ExceptionConstructor) {
    raytracer::exception::Exception exc("Test error message");
    ASSERT_NE(exc.what(), nullptr);
    ASSERT_TRUE(std::string(exc.what()).find("Test error message") !=
                std::string::npos);
}

TEST_F(ExceptionTest, ExceptionMessage) {
    raytracer::exception::Exception exc("Custom message");
    std::string message(exc.what());
    ASSERT_EQ(message.find("Custom message"), 0);
}

TEST_F(ExceptionTest, ExceptionLocation) {
    raytracer::exception::Exception exc("Test location",
                                        std::source_location::current());
    const auto& location = exc.location();
    ASSERT_NE(location.file_name(), nullptr);
    ASSERT_GT(location.line(), 0);
}

TEST_F(ExceptionTest, ExceptionCopyConstructor) {
    raytracer::exception::Exception exc1("Original message");
    raytracer::exception::Exception exc2 = exc1;
    ASSERT_STREQ(exc1.what(), exc2.what());
}

TEST_F(ExceptionTest, ExceptionMoveConstructor) {
    raytracer::exception::Exception exc1("Move message");
    raytracer::exception::Exception exc2 = std::move(exc1);
    std::string message(exc2.what());
    ASSERT_TRUE(message.find("Move message") != std::string::npos);
}

TEST_F(ExceptionTest, ExceptionCopyAssignment) {
    raytracer::exception::Exception exc1("First message");
    raytracer::exception::Exception exc2("Second message");
    exc2 = exc1;
    ASSERT_STREQ(exc1.what(), exc2.what());
}

TEST_F(ExceptionTest, ExceptionMoveAssignment) {
    raytracer::exception::Exception exc1("Move assignment message");
    raytracer::exception::Exception exc2("Other message");
    exc2 = std::move(exc1);
    std::string message(exc2.what());
    ASSERT_TRUE(message.find("Move assignment message") != std::string::npos);
}

TEST_F(ExceptionTest, ExceptionIsStdException) {
    raytracer::exception::Exception exc("Test");
    ASSERT_TRUE(dynamic_cast<std::exception*>(&exc) != nullptr);
}

// Tests for CoreException class
TEST_F(ExceptionTest, CoreExceptionSimpleMessage) {
    raytracer::exception::CoreException exc("Database connection failed");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Core error]:") != std::string::npos);
    ASSERT_TRUE(message.find("Database connection failed") !=
                std::string::npos);
}

TEST_F(ExceptionTest, CoreExceptionWithFormat) {
    raytracer::exception::CoreException exc("Error code: {}", 42);
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Core error]:") != std::string::npos);
    ASSERT_TRUE(message.find("Error code: 42") != std::string::npos);
}

TEST_F(ExceptionTest, CoreExceptionMultipleArgs) {
    raytracer::exception::CoreException exc("File {} not found at line {}",
                                            "config.txt", 123);
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Core error]:") != std::string::npos);
    ASSERT_TRUE(message.find("config.txt") != std::string::npos);
    ASSERT_TRUE(message.find("123") != std::string::npos);
}

TEST_F(ExceptionTest, CoreExceptionInheritance) {
    raytracer::exception::CoreException exc("Test");
    ASSERT_TRUE(dynamic_cast<raytracer::exception::Exception*>(&exc) !=
                nullptr);
    ASSERT_TRUE(dynamic_cast<std::exception*>(&exc) != nullptr);
}

// Tests for ParsingException class
TEST_F(ExceptionTest, ParsingExceptionSimpleMessage) {
    raytracer::exception::ParsingException exc("Invalid JSON format");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Parsing error]:") != std::string::npos);
    ASSERT_TRUE(message.find("Invalid JSON format") != std::string::npos);
}

TEST_F(ExceptionTest, ParsingExceptionWithFormat) {
    raytracer::exception::ParsingException exc(
        "Expected token {} at position {}", "EOF", 512);
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Parsing error]:") != std::string::npos);
    ASSERT_TRUE(message.find("EOF") != std::string::npos);
    ASSERT_TRUE(message.find("512") != std::string::npos);
}

TEST_F(ExceptionTest, ParsingExceptionThrow) {
    ASSERT_THROW(throw raytracer::exception::ParsingException("Parse error"),
                 raytracer::exception::Exception);
}

TEST_F(ExceptionTest, ParsingExceptionInheritance) {
    raytracer::exception::ParsingException exc("Test");
    ASSERT_TRUE(dynamic_cast<raytracer::exception::Exception*>(&exc) !=
                nullptr);
}

// Tests for PluginException class
TEST_F(ExceptionTest, PluginExceptionSimpleMessage) {
    raytracer::exception::PluginException exc("Plugin loading failed");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin error]:") != std::string::npos);
    ASSERT_TRUE(message.find("Plugin loading failed") != std::string::npos);
}

TEST_F(ExceptionTest, PluginExceptionWithPrefix) {
    raytracer::exception::PluginException exc("Plugin {} version mismatch",
                                              "sphere");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin error]:") != std::string::npos);
    ASSERT_TRUE(message.find("sphere") != std::string::npos);
}

TEST_F(ExceptionTest, PluginExceptionThrow) {
    ASSERT_THROW(throw raytracer::exception::PluginException("Plugin error"),
                 raytracer::exception::Exception);
}

// Tests for PrimitivesException class
TEST_F(ExceptionTest, PrimitivesExceptionSimpleMessage) {
    raytracer::exception::PrimitivesException exc("Sphere not created");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Primitive]:") != std::string::npos);
    ASSERT_TRUE(message.find("Sphere not created") != std::string::npos);
}

TEST_F(ExceptionTest, PrimitivesExceptionWithFormat) {
    raytracer::exception::PrimitivesException exc("Invalid radius: {}", -5.0);
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Primitive]:") != std::string::npos);
    ASSERT_TRUE(message.find("-5") != std::string::npos);
}

TEST_F(ExceptionTest, PrimitivesExceptionThrow) {
    ASSERT_THROW(
        throw raytracer::exception::PrimitivesException("Primitive error"),
        raytracer::exception::PluginException);
}

TEST_F(ExceptionTest, PrimitivesExceptionInheritance) {
    raytracer::exception::PrimitivesException exc("Test");
    ASSERT_TRUE(dynamic_cast<raytracer::exception::PluginException*>(&exc) !=
                nullptr);
    ASSERT_TRUE(dynamic_cast<raytracer::exception::Exception*>(&exc) !=
                nullptr);
}

// Tests for TransformationException class
TEST_F(ExceptionTest, TransformationExceptionSimpleMessage) {
    raytracer::exception::TransformationException exc("Rotation failed");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Transformation]:") !=
                std::string::npos);
    ASSERT_TRUE(message.find("Rotation failed") != std::string::npos);
}

TEST_F(ExceptionTest, TransformationExceptionWithFormat) {
    raytracer::exception::TransformationException exc(
        "Invalid angle: {} degrees", 360.5);
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Transformation]:") !=
                std::string::npos);
    ASSERT_TRUE(message.find("360") != std::string::npos);
}

TEST_F(ExceptionTest, TransformationExceptionThrow) {
    ASSERT_THROW(throw raytracer::exception::TransformationException(
                     "Transformation error"),
                 raytracer::exception::PluginException);
}

TEST_F(ExceptionTest, TransformationExceptionInheritance) {
    raytracer::exception::TransformationException exc("Test");
    ASSERT_TRUE(dynamic_cast<raytracer::exception::PluginException*>(&exc) !=
                nullptr);
    ASSERT_TRUE(dynamic_cast<raytracer::exception::Exception*>(&exc) !=
                nullptr);
}

// Tests for LightException class
TEST_F(ExceptionTest, LightExceptionSimpleMessage) {
    raytracer::exception::LightException exc("Light creation failed");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Light]:") != std::string::npos);
    ASSERT_TRUE(message.find("Light creation failed") != std::string::npos);
}

TEST_F(ExceptionTest, LightExceptionWithFormat) {
    raytracer::exception::LightException exc("Invalid intensity: {}", -10);
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Light]:") != std::string::npos);
    ASSERT_TRUE(message.find("-10") != std::string::npos);
}

TEST_F(ExceptionTest, LightExceptionThrow) {
    ASSERT_THROW(throw raytracer::exception::LightException("Light error"),
                 raytracer::exception::PluginException);
}

TEST_F(ExceptionTest, LightExceptionInheritance) {
    raytracer::exception::LightException exc("Test");
    ASSERT_TRUE(dynamic_cast<raytracer::exception::PluginException*>(&exc) !=
                nullptr);
    ASSERT_TRUE(dynamic_cast<raytracer::exception::Exception*>(&exc) !=
                nullptr);
}

// Tests for MaterialException class
TEST_F(ExceptionTest, MaterialExceptionSimpleMessage) {
    raytracer::exception::MaterialException exc("Material loading failed");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Material]:") != std::string::npos);
    ASSERT_TRUE(message.find("Material loading failed") != std::string::npos);
}

TEST_F(ExceptionTest, MaterialExceptionWithFormat) {
    raytracer::exception::MaterialException exc("Unknown material: {}",
                                                "unknown_type");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Plugin > Material]:") != std::string::npos);
    ASSERT_TRUE(message.find("unknown_type") != std::string::npos);
}

TEST_F(ExceptionTest, MaterialExceptionThrow) {
    ASSERT_THROW(
        throw raytracer::exception::MaterialException("Material error"),
        raytracer::exception::PluginException);
}

TEST_F(ExceptionTest, MaterialExceptionInheritance) {
    raytracer::exception::MaterialException exc("Test");
    ASSERT_TRUE(dynamic_cast<raytracer::exception::PluginException*>(&exc) !=
                nullptr);
    ASSERT_TRUE(dynamic_cast<raytracer::exception::Exception*>(&exc) !=
                nullptr);
}

// Exception hierarchy tests
TEST_F(ExceptionTest, ExceptionHierarchyCoreException) {
    ASSERT_THROW(throw raytracer::exception::CoreException("Test"),
                 raytracer::exception::Exception);
}

TEST_F(ExceptionTest, ExceptionHierarchyParsingException) {
    ASSERT_THROW(throw raytracer::exception::ParsingException("Test"),
                 std::exception);
}

TEST_F(ExceptionTest, ExceptionHierarchyPluginException) {
    ASSERT_THROW(throw raytracer::exception::PluginException("Test"),
                 std::exception);
}

// Edge case tests
TEST_F(ExceptionTest, ExceptionEmptyMessage) {
    raytracer::exception::Exception exc("");
    ASSERT_NE(exc.what(), nullptr);
}

TEST_F(ExceptionTest, CoreExceptionEmptyMessage) {
    raytracer::exception::CoreException exc("");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Core error]:") != std::string::npos);
}

TEST_F(ExceptionTest, ExceptionLongMessage) {
    std::string longMsg(1000, 'a');
    raytracer::exception::Exception exc(longMsg);
    std::string message(exc.what());
    ASSERT_TRUE(message.find(longMsg) != std::string::npos);
}

TEST_F(ExceptionTest, ParsingExceptionSpecialCharacters) {
    raytracer::exception::ParsingException exc(
        "Invalid character: '\\n' in config");
    std::string message(exc.what());
    ASSERT_TRUE(message.find("[Parsing error]:") != std::string::npos);
}
