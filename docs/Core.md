# Raytracer - Core

## Description
**TODO**

## Architecture
**TODO**

...

## Exceptions
The system is based on a hierarchy of classes allowing specialization of errors by module (Parsing, Plugins, Primitives, etc.).

- `BaseException` - Base class for all custom exceptions
- `ParsingException` - Errors during file/configuration parsing
- `PluginsException` - Errors related to plugin loading and execution
- `PrimitivesException` - Errors in primitive object creation/manipulation
- `RenderingException` - Errors during rendering process

### Template-Based String Formatting
The exception classes use variadic templates to support formatted error messages similar to `printf` or modern string formatting libraries. This approach eliminates the need for manual string concatenation and type conversion.
This allows developers to create detailed and informative error messages with ease, improving debugging and user feedback.

**Example :**
```cpp
throw ParsingException("Failed to parse file: {} at line {}", filename.c_str(), lineNumber);
```
