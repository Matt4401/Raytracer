# RayTracer Configuration File Format

This document explains how to write valid configuration files for the RayTracer project. Configuration files are parsed using libconfig and must follow specific rules to be properly converted into objects.

## Overview

The configuration parser processes `.cfg` files and creates a scene by:

1. **Loading `sceneParameters`** - Required base configuration for the scene
2. **Creating objects** - Using explicit name and parameters structure
3. **Assigning materials** - Optional material definitions with their own parameters
4. **Type safety** - All values must have explicit types (strings with `""`, floats with `.0`, etc.)
5. **Enum handling** - Enum values represented as strings and converted by plugins

## Core Rules

### Rule 1: SceneParameters is Mandatory

Every configuration file must start with a `sceneParameters` group. This is the foundation for creating the scene and contains all required lighting and rendering settings.

```cfg
sceneParameters:
{
    ambiantOcclusion: {
        samples = 0;
        radius = 0.0;
    };
    ambientLight: {
        color: { r = 0; g = 0; b = 0; };
        intensity = 0.0;
    };
    ambientDiffuse: {
        color: { r = 0; g = 0; b = 0; };
        intensity = 0.0;
    };
};
```

**All fields in `sceneParameters` are mandatory** and required to initialize the scene properly.

### Rule 2: Objects Have Name and Parameters

Objects are defined with two mandatory components:
- **`name`**: A string identifying the object type (e.g., `"sphere"`, `"plane"`)
- **`parameters`**: A group containing all object-specific properties

```cfg
{
    name = "sphere";
    parameters: {
        center: { x = 50.0; y = 40.8; z = 81.6; };
        radius = 100000.0;
    };
}
```

Objects can optionally include a **`material`** section with the same structure:

```cfg
material: {
    name = "flatcolor";
    parameters: {
        color: { r = 191; g = 63; b = 63; };
        reflType = "DIFF";
        emission: { x = 0.0; y = 0.0; z = 0.0; };
        reflectivity = 0.0;
        transparency = 0.0;
        ior = 1.0;
        roughness = 0.0;
        metalness = 0.0;
    };
};
```

### Rule 3: Two Ways to Define Objects

#### Method 1: Individual Objects (Group Style)

Each object is a complete group with `name` and `parameters`:

```cfg
primitives:
(
    {
        name = "sphere";
        parameters: {
            center: { x = 100001.0; y = 40.8; z = 81.6; };
            radius = 100000.0;
        };
        material: {
            name = "flatcolor";
            parameters: {
                color: { r = 191; g = 63; b = 63; };
                reflType = "DIFF";
                reflectivity = 0.0;
            };
        };
    },
    {
        name = "sphere";
        parameters: {
            center: { x = -99901.0; y = 40.8; z = 81.6; };
            radius = 100000.0;
        };
        material: {
            name = "flatcolor";
            parameters: {
                color: { r = 63; g = 63; b = 191; };
                reflType = "DIFF";
                reflectivity = 0.0;
            };
        };
    }
);
```

#### Method 2: List Parameters (Direct Style)

For certain object types (like lights), each list element directly contains the object parameters without a wrapper `name` field. The type is inferred from the list name (singular form):

```cfg
lights:
{
    point_light = (
        {
            position: { x = 50.0; y = 57.1; z = 81.6; };
            color: { r = 250; g = 230; b = 230; };
            intensity = 400.0;
            radius = 2.0;
        },
        {
            position: { x = 100.0; y = 100.0; z = 100.0; };
            color: { r = 255; g = 255; b = 255; };
            intensity = 200.0;
            radius = 3.0;
        }
    );
};
```

In this example:
- List name: `point_light`
- Object type: `point_light` (used by the plugin constructor)
- One object created per list element

Be careful this second method do not allow materials

### Rule 4: Type Safety - Explicit Type Declarations

All values must be explicitly typed. The parser and plugins rely on correct type representation:

| Type | Example | Format |
|------|---------|--------|
| String | `"sphere"` | Always with double quotes |
| Float/Double | `100000.0` or `3.14` | Must include `.0` for floats |
| Integer | `42` or `0` | No decimal point |
| Boolean | `true` or `false` | Lowercase |
| Vector/Group | `{ x = 0.0; y = 0.0; z = 0.0; }` | Semicolons between fields |
| Enum (as string) | `"DIFF"`, `"SPEC"`, `"REFR"` | Quoted strings |

**Correct Examples:**
```cfg
radius = 100000.0;           # ✓ Float
samples = 0;                 # ✓ Integer
color = "red";               # ✓ String
reflType = "DIFF";           # ✓ Enum as string
emission: { x = 0.0; y = 0.0; z = 0.0; };  # ✓ Vector
```

**Incorrect Examples:**
```cfg
radius = 100000;             # ✗ Missing .0 for float
samples = 0.0;               # ✗ Should be integer
color = red;                 # ✗ Missing quotes
reflType = DIFF;             # ✗ Enum must be quoted
emission: { x = 0; y = 0; z = 0; };  # ✗ Floats need .0
```

### Rule 5: Enum Values as Strings

Some material parameters accept enum values. These must be represented as **quoted strings**, and the plugin constructor will convert them to the appropriate enum type.

**Common Enum Examples:**
- Reflection type: `"DIFF"` (diffuse), `"SPEC"` (specular), `"REFR"` (refraction)

```cfg
material: {
    name = "flatcolor";
    parameters: {
        color: { r = 191; g = 63; b = 63; };
        reflType = "DIFF";      # Converted to enum by plugin
        reflectivity = 0.0;
        transparency = 0.0;
    };
};
```

### Rule 6: Unused Parameters are Ignored

Parameters that are not handled by a plugin constructor are safely ignored. This allows for flexible configuration files where some optional parameters can be present but unused.

```cfg
material: {
    name = "simpleMaterial";
    parameters: {
        color: { r = 255; g = 0; b = 0; };
        
        # These fields might not be used by all plugins
        roughness = 0.5;        # Ignored if not supported
        metalness = 0.8;        # Ignored if not supported
        customParam = "value";  # Ignored if not supported
    };
};
```

### Rule 7: Objects Clusters Config for Object Extension

A special configuration key, `objects_clusters`, is used to include additional objects into the current scene by referencing external configuration files. Unlike `scenes` which creates separate scenes, `objects_clusters` extends the current scene with more objects. The `objects_clusters` config must be either a list or an array of strings, where each string represents a relative path to a configuration file containing only objects.

**Example:**
```cfg
objects_clusters = [
    "walls.cfg",
    "furniture.cfg"
];
```

or

```cfg
objects_clusters = (
    "walls.cfg",
    "furniture.cfg"
);
```

**Important Notes:**
- Each referenced file should contain only object definitions (primitives, lights, etc.) and **must NOT contain** `sceneParameters` or `scenes` declarations. They will be considered as object in those files.

## Small Error Cases and Corrections

### ❌ Error Case 1: Missing SceneParameters

**WRONG:**
```cfg
camera:
(
    { resolution: { width = 400; height = 200; }; }
);
```

**Why:** The scene cannot be initialized without `sceneParameters`.

**CORRECT:**
```cfg
sceneParameters:
{
    ambiantOcclution: { samples = 0; radius = 0.0; };
    ambientLight: { color: { r = 0; g = 0; b = 0; }; intensity = 0.0; };
    ambientDiffuse: { color: { r = 0; g = 0; b = 0; }; intensity = 0.0; };
};

camera:
(
    { resolution: { width = 400; height = 200; }; }
);
```

### ❌ Error Case 2: Incorrect Type (Missing Float Indicator)

**WRONG:**
```cfg
primitives:
(
    {
        name = "sphere";
        parameters: {
            center: { x = 50; y = 40; z = 80; };      # Missing .0
            radius = 16.5;
        };
    }
);
```

**Why:** Integer values are treated differently than floats. Coordinates should be floats.

**CORRECT:**
```cfg
primitives:
(
    {
        name = "sphere";
        parameters: {
            center: { x = 50.0; y = 40.0; z = 80.0; };
            radius = 16.5;
        };
    }
);
```

### ❌ Error Case 3: Mixing Styles (Group Style Inside List Parameters)

**WRONG:**
```cfg
lights:
{
    point_light = (
        {
            name = "point_light";        # Wrong! List parameters don't use name
            parameters: {
                position: { x = 50.0; y = 57.1; z = 81.6; };
                intensity = 400.0;
            };
        }
    );
};
```

**Why:** List parameters (Method 2) don't use the `name`/`parameters` wrapper. Each element is directly the parameters.

**CORRECT:**
```cfg
lights:
{
    point_light = (
        {
            position: { x = 50.0; y = 57.1; z = 81.6; };
            color: { r = 250; g = 230; b = 230; };
            intensity = 400.0;
            radius = 2.0;
        }
    );
};
```

### ❌ Error Case 4: Inconsistent Type Casting

**WRONG:**
```cfg
material: {
    name = "flatcolor";
    parameters: {
        color: { r = 191.0; g = 63.0; b = 63.0; };  # Should be integers
        reflectivity = 0;                           # Should be 0.0
        transparency = true;                        # Should be 0.0 or 1.0
    };
};
```

**Why:** Color values are typically integers (0-255), and floats should be explicitly declared.

**CORRECT:**
```cfg
material: {
    name = "flatcolor";
    parameters: {
        color: { r = 191; g = 63; b = 63; };
        reflectivity = 0.0;
        transparency = 0.0;
    };
};
```
