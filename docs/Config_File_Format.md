# RayTracer Configuration File Format

This document explains how to write valid configuration files for the RayTracer project. Configuration files are parsed using libconfig and must follow specific rules to be properly converted into objects.

## Overview

The configuration parser processes `.cfg` files and creates objects based on the file structure. The parser uses the following principles:

1. **Root-level groups** are treated as object types (e.g., `camera`, `primitives`, `lights`)
2. **Lists** within a type create multiple objects of the same kind
3. **Groups** within a type create unique/singleton objects
4. **Parser errors** throw `raytracer::exception::ParsingException`

## Core Rules

### Rule 1: Root-Level Names Define Types

The names at the root level of your configuration file represent object types:

```cfg
camera { ... }        # Type: camera
primitives { ... }    # Type: primitives
lights { ... }        # Type: lights
```

Each root-level group will be processed as a separate type.

### Rule 2: Groups Without Lists are Single Objects

If a type contains **no lists** `()`, it is treated as a single object and built immediately.

```cfg
camera:
{
    resolution = { width = 1920; height = 1080; };
    position = { x = 0; y = -100; z = 20; };
    fieldOfView = 72.0;
};
```

In this example:
- The type is `camera`
- All properties are collected into a map
- A single object is built with the type name `"camera"`

### Rule 3: Lists Create Multiple Objects

Lists (denoted with `()`) within a type create multiple objects of the same kind:

```cfg
primitives:
{
    spheres = (
        { x = 60; y = 5; z = 40; r = 25; },
        { x = -40; y = 20; z = -10; r = 35; }
    );
};
```

In this example:
- The type is `primitives`
- The list name is `spheres`
- Two objects are created, each with type name `"sphere"` (singular form of the list name)
- Each object in the list is passed to the builder

### Rule 4: Mixed Content (Lists + Groups)

When a type contains **both lists and non-list groups**, the parser enters mixed mode:

```cfg
lights:
{
    ambient = { multiplier = 0.4; };
    diffuse = { multiplier = 0.6; };
    
    point = (
        { x = 400; y = 100; z = 500; },
        { x = 500; y = 200; z = 600; }
    );
};
```

**Behavior:**
1. Lists are processed first (e.g., `point` creates objects with type name `"point"`)
2. Non-list groups are then processed as separate unique objects with their own names
3. In this example:
   - Two objects of type `"point"` are created (one for each list element)
   - One object of type `"ambient"` is created
   - One object of type `"diffuse"` is created

### Rule 5: Error Handling

Any parsing error will throw a `raytracer::exception::ParsingException` with a descriptive message. Common errors include:

- **Invalid file path**: File doesn't exist or can't be read
- **Invalid syntax**: Malformed libconfig syntax
- **Invalid type casting**: Non-group value in mixed mode (Rule 4)
- **Build failure**: Object builder returns nullptr

## Configuration Examples

### Example 1: Simple Single Object (Camera)

```cfg
camera:
{
    resolution = { width = 1920; height = 1080; };
    position = { x = 0; y = -100; z = 20; };
    rotation = { x = 0; y = 0; z = 0; };
    fieldOfView = 72.0;
};
```

**Parsing Result:**
- Type name: `"camera"`
- Parameters passed to builder:
  ```
  {
    "resolution": { width: 1920, height: 1080 },
    "position": { x: 0, y: -100, z: 20 },
    "rotation": { x: 0, y: 0, z: 0 },
    "fieldOfView": 72.0
  }
  ```

### Example 2: Multiple Objects from a List (Spheres)

```cfg
primitives:
{
    spheres = (
        {
            x = 60;
            y = 5;
            z = 40;
            r = 25;
            color = { r = 255; g = 64; b = 64; };
        },
        {
            x = -40;
            y = 20;
            z = -10;
            r = 35;
            color = { r = 64; g = 255; b = 64; };
        }
    );
};
```

**Parsing Result:**
- Two objects are created
- Both have type name: `"sphere"` (singular)
- Each receives its respective parameters

### Example 3: Mixed Lists and Groups (Lights)

```cfg
lights:
{
    ambient = { multiplier = 0.4; };
    diffuse = { multiplier = 0.6; };
    
    point = (
        { x = 400; y = 100; z = 500; },
        { x = 500; y = 200; z = 600; }
    );
};
```

**Parsing Result:**
1. Process lists first:
   - Two objects of type `"point"` (singular) are created
2. Process groups:
   - One object of type `"ambient"` is created with `{ "multiplier": 0.4 }`
   - One object of type `"diffuse"` is created with `{ "multiplier": 0.6 }`

### Example 4: Complete Configuration File

```cfg
camera:
{
    resolution = { width = 1920; height = 1080; };
    position = { x = 0; y = -100; z = 20; };
    rotation = { x = 0; y = 0; z = 0; };
    fieldOfView = 72.0;
};

primitives:
{
    spheres = (
        {
            x = 60;
            y = 5;
            z = 40;
            r = 25;
            color = { r = 255; g = 64; b = 64; };
        },
        {
            x = -40;
            y = 20;
            z = -10;
            r = 35;
            color = { r = 64; g = 255; b = 64; };
        }
    );

    planes = (
        {
            axis = "Z";
            position = -20;
            color = { r = 64; g = 64; b = 255; };
        }
    );
};

lights:
{
    ambient = { multiplier = 0.4; };
    diffuse = { multiplier = 0.6; };

    point = (
        { x = 400; y = 100; z = 500; }
    );
};
```

## Supported Value Types

The parser supports the following libconfig value types:

| Type | Example | Parsed As |
|------|---------|-----------|
| Integer | `42` | `int` |
| 64-bit Integer | `9223372036854775807` | `long long` |
| Float/Double | `3.14` or `72.0` | `double` |
| String | `"text"` | `std::string` |
| Boolean | `true` or `false` | `bool` |
| Group | `{ key = value; }` | `std::map<std::string, std::any>` |
| Array | `[1, 2, 3]` | `std::vector<std::any>` |
| List | `(...)` | Processed as multiple objects |

## Naming Conventions

- **Type names** (root level): Use descriptive names that match your object factory registrations
- **List names**: The singular form will be used as the type name for each object in the list
  - `spheres` → type name `"sphere"`
  - `lights` → type name `"light"`
  - `cameras` → type name `"camera"`

## Common Mistakes to Avoid

### ❌ Mistake 1: Mixing List and Non-List Elements Incorrectly

```cfg
# WRONG - This will cause an error because "color" is not a group
primitives:
{
    color = 255;
    spheres = ( { x = 0; y = 0; z = 0; } );
};
```

**Why:** In mixed mode, all non-list elements must be groups.

### ❌ Mistake 2: Forgetting the Colon in Libconfig Syntax

```cfg
# WRONG - Missing colon after key
camera { ... }

# CORRECT
camera: { ... }
```

### ❌ Mistake 3: Using Array Instead of List

```cfg
# WRONG - Arrays don't create multiple objects
primitives:
{
    spheres = [ { x = 0 }, { x = 1 } ];
};

# CORRECT - Use lists
primitives:
{
    spheres = ( { x = 0 }, { x = 1 } );
};
```

### ❌ Mistake 4: Invalid Nested Syntax

```cfg
# WRONG - Cannot use semicolons inside {} at libconfig array level
primitives: { spheres = ( x = 0; ); };

# CORRECT - Each element must be a complete group
primitives:
{
    spheres = ( { x = 0; } );
};
```
