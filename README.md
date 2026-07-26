# Skyblock Companion

A modular C++ companion application for Hypixel SkyBlock.

The current feature set focuses on Jacob's Farming Contests:

- Live contest schedule from a community API
- Crop-based filtering with `Any` and `All` match modes
- Real-time countdowns
- Scrollable contest list with incremental loading
- Custom font selection
- Reusable core library separated from the raylib interface

## Project structure

```text
include/skyblock/     Public reusable C++ API
src/core/             Contest filtering and queries
src/providers/        Contest data providers
src/app/              Raylib desktop application
assets/fonts/         Local font files, excluded from Git
```

## Building

Requirements:

- C++20 compiler
- CMake
- Ninja
- raylib
- libcurl
- nlohmann/json

Configure and build:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

Run:

```powershell
.\build\SkyblockCompanion.exe
```

## Reusing the core library

Other C++ targets can link against `Skyblock::Core`:

```cmake
add_executable(AnotherProgram src/another/main.cpp)
target_link_libraries(AnotherProgram PRIVATE Skyblock::Core)
```

Then include the public API:

```cpp
#include <skyblock/SkyblockCore.h>
```

The reusable library contains no raylib types, so the contest models, provider interface, filtering, and query logic can be used by other applications.

## Font assets

Font files are not committed to the repository. Place the required `.ttf` and `.otf` files in `assets/fonts/` before building.
