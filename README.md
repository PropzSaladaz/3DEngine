# 3D Engine

Custom 3D engine built with OpenGL that renders 3D scenes from a C++ codebase. The repository contains the engine sources, external libraries, and sample assets needed to build and run the application across platforms.

## Project structure
- `examples` – example projects (not required for building the engine)
- `libs` – external libraries and dependencies
- `out` – build directory
- `resources` – models, textures, and other assets
- `src` & `include` – engine source files and headers
- `test` – test files

## Building and running

### Visual Studio 2022 (Windows)
1. Open the repository folder in Visual Studio.
2. Allow Visual Studio to detect and generate the CMake configuration.
3. Use **Build → Build All** to compile the project.
4. Select `engine.exe` as the startup item and start debugging or running to launch the application.

### Linux (CMake)
1. Configure the build:
   ```bash
   cmake -S . -B build
   ```
2. Compile the project:
   ```bash
   cmake --build build
   ```
3. Run the compiled engine from the build output (binary name may vary based on configuration).
