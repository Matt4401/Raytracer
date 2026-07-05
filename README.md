# Raytracer

> ### ℹ️ About this repository
> **This is not the original repository.** The original project was developed on
> a **private** repository hosted on the Epitech GitHub organization
> (`EpitechPGE2-2025/G-OOP-400-NAN-4-1-raytracer-3`) and cannot be shared
> publicly.
>
> This repository is a **public copy (mirror)** of that private original,
> published so the work can be showcased and consulted. Because it is a copy,
> some organization-specific data (issues, pull requests, CI secrets, access
> rights, etc.) may be missing or behave differently from the original.

A physically-inspired **ray tracer** written in modern **C++20**. It generates
realistic images by simulating the inverse path of light, and renders a scene
described in an external configuration file to a **PPM** image (or displays it
live through an **SFML** window).

```
$> ./raytracer --help
USAGE: ./raytracer <SCENE_FILE>
	SCENE_FILE: scene configuration
	-e: export mode (ppm)
	-v: visual mode (cli/sfml)
```

---

## Table of contents

- [Features](#features)
- [Architecture](#architecture)
- [Design patterns](#design-patterns)
- [Building](#building)
- [Usage](#usage)
- [Scene file format](#scene-file-format)
- [Project layout](#project-layout)
- [Testing](#testing)

---

## Features

### Primitives
- Sphere, Plane
- Cylinder & **limited** cylinder
- Cone & **limited** cone
- Torus, Tanglecube
- Triangle & **`.OBJ`** meshes (with material import)
- Rectangle
- **Fractals**: Mandelbulb, Menger sponge, Koch snowflake (3D)

### Transformations
- Translation, Rotation, Scale, Shear
- 4×4 transformation matrices (composable via a decorator chain)

### Lights
- Ambient light & ambient diffuse
- Directional lights (multiple)
- Point lights (multiple)
- Spot lights
- Colored lights
- Drop shadows
- Ambient occlusion

### Materials & optical effects
- Flat color
- Reflection / metalness / roughness (specular)
- Transparency & refraction (index of refraction)
- Emission
- Normal mapping
- Texture mapping (from image file)
- Procedural textures: chessboard, Perlin-based (marble, wood, magma, cracked
  dirt), Worley noise, Turing pattern

### Rendering & optimization
- **BVH** acceleration structure with pluggable split strategies
  (SAH, median, midpoint)
- Multithreaded rendering
- Fast scene **preview** renderer
- Live **file-watch** auto-reload of the scene

### Interface
- Headless export to **PPM**
- **SFML** viewer (scene selection, live image display)
- CLI visual mode

---

## Architecture

The engine is built around a set of **interfaces / abstract bases** so features
can be added without rewriting the core:

- `IObject` / `AObject` — base of everything placed in a scene
- `IPrimitive` / `APrimitive` — renderable geometry
- `ILight` / `ALight` — light sources
- `IMaterial` / `AMaterial` — surface shading
- `IScene` / `AScene` — scene container
- `ICamera` / `ACamera` — viewpoint
- `IVisual` — output backends (`CliVisual`, `SFMLVisual`)
- `IExport` — image exporters (`ExportPPM`)

### Plugin system
Every primitive, light, material, transformation and the scene itself is
compiled as a **shared library (`.so`) plugin** loaded at runtime. The main
executable is **not** linked against any plugin; they live in the `./plugins/`
directory and are loaded through an encapsulated `libdl` wrapper (`DlLoader`,
`PluginManager`, `ObjectFactory`).

---

## Design patterns

Among others, the project uses:

- **Factory** — `ObjectFactory` builds objects from plugin symbols;
  `StrategyFactory` builds BVH split strategies.
- **Decorator** — `APrimitiveDecorator` / `ATransformDecorator` wrap primitives
  to apply transformations (translation, rotation, scale, shear).
- **Strategy** — interchangeable BVH split strategies (`ISplitStrategy`) and
  fractal generation strategies (`IFractalStrategy`).
- **Builder** — `FractalBuilder` assembles fractal primitives.
- **Composite** — a `Scene` aggregates objects and is treated uniformly.

---

## Building

### Requirements
- A C++20 compiler
- **CMake ≥ 3.28**
- **SFML 2** (graphics, window, system)
- `libconfig++` — fetched and built automatically via CMake `FetchContent`

### Build
```bash
mkdir -p build && cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
```

This produces the `raytracer` binary at the **root** of the repository and the
plugins in `./plugins/`:

```bash
$> ls ./raytracer ./plugins/
./raytracer

./plugins/:
sphere_plug.so   plane_plug.so   cone_plug.so   cylinder_plug.so   ...
```

---

## Usage

```bash
# Render a scene to a PPM file (export mode)
./raytracer scenes/40_light_point.cfg -e ppm

# Open the SFML viewer
./raytracer scenes/40_light_point.cfg -v sfml

# Show help
./raytracer --help
```

On error, messages are printed to the standard error output and the program
exits with code **84** (0 on success).

Ready-made scenes live in [`scenes/`](scenes) and reference renders in
[`screenshots/`](screenshots).

---

## Scene file format

Scenes use the **libconfig++** format. A scene declares camera(s), global scene
parameters (ambient light, ambient occlusion), primitives (each with a material)
and lights. Simplified excerpt:

```conf
camera:
(
    {
        resolution: { width = 400; height = 200; };
        position:   { x = 50.0; y = 52.0; z = 295.6; };
        rotation:   { x = 0.0; y = -0.042612; z = -1.0; };
        fieldOfView = 90.0;
    }
);

primitives:
(
    {
        name = "sphere";
        parameters: { center: { x = 50.0; y = 40.8; z = 81.6; }; radius = 16.5; };
        material: {
            name = "flatcolor";
            parameters: {
                color: { r = 191; g = 63; b = 63; };
                reflType = "DIFF";
                reflectivity = 0.0; transparency = 0.0; ior = 1.0;
                roughness = 0.0;    metalness = 0.0;
            };
        };
    }
);
```

See [`docs/Config_File_Format.md`](docs/Config_File_Format.md) for the full
specification and [`base.cfg`](base.cfg) for a complete example.

---

## Project layout

```
include/        # Public interfaces & abstract bases (objects, math, bvh, exceptions)
src/            # Core engine: parsing, rendering, BVH, plugin loading, visual/export
libs/           # Plugins (primitives, lights, materials, transformations, scene)
plugins/        # Build output: compiled .so plugins
scenes/         # Example scene configuration files
screenshots/    # Reference renders (.ppm)
assets/         # Models, textures, fonts, normal maps
docs/           # Core / plugin / config-file documentation
tests/          # Unit tests
```

---

## Testing

Unit tests are built with the project and run through CTest:

```bash
cd build
cmake --build . --target check     # or: ctest --output-on-failure
```
