# Raytracer - Creating a Plugin

## Overview

Plugins are shared libraries that extend the raytracer's functionality. The architecture is based on a minimal interface and a dynamic loading system that makes it easy to add new components (primitives, materials, lights, transformations, etc.) without recompiling the core system.

## Architecture

### Object Type Hierarchy

All plugins create objects that inherit from `IObject` and have a specific type. Interface defined in `include/object/IObject.hpp`.

### Plugin Contract (Extern C)

Each plugin must export **three C functions** that allow the system to operate correctly. These functions must be declared with `extern "C"` to prevent C++ name mangling.

#### 1. **`type()`** - Plugin type

```cpp
extern "C" const char *type(void) {
    return "primitive";  // or "light", "material", etc.
}
```

#### 2. **`name()`** - Unique plugin name

```cpp
extern "C" const char *name(void) {
    return "sphere";  // Must be unique
}
```

#### 3. **`builder()`** - Factory function

```cpp
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &sphere_builder;
}
```

```cpp
using BuilderFunc =
    raytracer::object::IObject *(*)(const std::vector<std::any> &);
```

**Returns**: a pointer to the factory function (cast via `BuilderFunc`) that creates instances.

> **Note**: `BuilderFunc` is defined in `IObject.hpp`

---

## Plugin Structure

### File Organization

```
libs/primitive/Example/
├── CMakeLists.txt          # Build configuration
├── Example.hpp             # Implementation class
└── ExamplePlugin.cpp       # Plugin entry point
```

### Header File: `Example.hpp`

Declares the class that will inherit from `IObject`:

```cpp
#pragma once

#include <iostream>
#include "object/IObject.hpp"

namespace raytracer::object {
    class Example : public IObject {
      public:
        Example() = default;
        ~Example() override = default;

        // IObject interface implementation
        Type type() const override {
            return Type::PRIMITIVE;
        }

        // Plugin-specific methods
        void work() {
            std::cout << "Example rendering...\n";
        }
    };
}
```

**Key points**:
- Inheritance from `IObject` is **mandatory**
- `type()` must be implemented and return the appropriate type
- Destructors must be properly marked as `virtual` (via `override`)

---

### Implementation File: `ExamplePlugin.cpp`

Plugin entry point containing the three exported functions:

```cpp
#include <any>
#include <string>
#include <vector>
#include "object/IObject.hpp"

// [1] Factory function that creates instances
extern "C" raytracer::object::IObject *exampleBuilder(
    const std::vector<std::any> &args) {
    // Validate and parse arguments
    // Create and return an instance
    return new raytracer::object::Example();
}

// [2] Declare the plugin type
extern "C" const char *type(void) {
    return "primitive";
}

// [3] Declare the unique plugin name
extern "C" const char *name(void) {
    return "sphere";
}

// [4] Return the factory function
extern "C" raytracer::object::BuilderFunc builder(void) {
    return &exampleBuilder;
}
```

> **Note**: The factory function (`exampleBuilder`) must be defined **before** the `builder()` function so that the function pointer is valid at the time it is returned.

---

### Build Configuration: `CMakeLists.txt`

Configures compilation as a shared library:

```cmake
cmake_minimum_required(VERSION 3.10)
SET(CMAKE_CXX_STANDARD 20)

# Plugin sources and headers
SET(SRCS
    ExamplePlugin.cpp
)

SET(HEADERS
    Example.hpp
)

# Create a shared library named "example_plug"
add_library(example_plug SHARED ${SRCS} ${HEADERS})

# Include headers from the main project
target_include_directories(example_plug PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Output configuration
set_target_properties(example_plug PROPERTIES
    PREFIX ""                                          # No "lib" prefix
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/  # Output directory
)
```

> **Important**: Do not forget to add `add_subdirectory()` for your plugin in the parent `CMakeLists.txt`, otherwise it will not be compiled.

---

## Error Handling

Use the system's exception hierarchy to report errors:

```cpp
#include "exception/Exception.hpp"

extern "C" raytracer::object::IObject *exampleBuilder(
    const std::vector<std::any> &args) {

    if (args.empty()) {
        throw raytracer::exception::Exception(
            "Example builder requires at least one argument");
    }

    try {
        float radius = std::any_cast<float>(args[0]);
        if (radius <= 0) {
            throw raytracer::exception::Exception(
                "Example radius must be positive, got: {}", radius);
        }
        return new raytracer::object::Example(radius);
    } catch (const std::bad_any_cast &) {
        throw raytracer::exception::Exception(
            "First argument must be a float for the radius");
    }
}
```
