# Blade And Stone 2

A top-down roguelike game built in C++. This project demonstrates the integration of modern C++ techniques with libraries like Raylib, Box2D, and LDtk. The main goal is to showcase the versatility of C++ in game development by creating an efficient, and extensible game capable of handling physics, rendering, and automated systems.

![BladeAndStone2Image](https://i.imgur.com/Fh7ytZA.png)

## Platforms & Downloads
- [Play on Itch.io](https://tlamir.itch.io/blade-and-stone-2) - Web assembly build or download
- [Windows Build](https://github.com/Tlamir/BladeAndStone2/releases/tag/Windows) - Direct download from GitHub

### 📺 Gameplay Video
Gameplay youtube link: https://www.youtube.com/watch?v=GlSLxQbG-0g

### 🛠️ Build from Source
For building the game yourself, check the [Building](#building) section below.
[Download Latest Release](https://github.com/Tlamir/BladeAndStone2/releases/tag/Windows)

## Features
- Top-down roguelike gameplay with automatic combat
- Level design powered by LDtk with automatic collision handling
- Physics-based movement and collisions using Box2D
- Dynamic enemy spawning system
- Dual Weapon System: Combine sword combat with a bullet-based magic system
- Debug visualization options for developers

## Built With
- **C++20**: Modern features like constexpr, ranges, and smart pointers
- **[Raylib](https://www.raylib.com/)** (v5.0): 2D graphics library
- **[Box2D](https://box2d.org/)** (v2.4.1): Physics engine
- **[LDtkLoader](https://github.com/Madour/LDtkLoader)** (v1.5.3.1): LDtk level editor integration
- **[{fmt}](https://github.com/fmtlib/fmt)** (v10.2.1): String formatting library

## Project Structure
```
├── sources/
│   ├── entities/      # Game entities
│   ├── physics/       # Box2D integration
│   ├── scenes/        # Scene management
│   ├── utils/         # Utilities
│   ├── Constants.hpp  # Game constants
├── assets/            # Game resources
└── CMakeLists.txt     # Build configuration
```

## Building

### Windows

I recommend using **CMake GUI** to generate Visual Studio project files for a smoother build process.

#### Prerequisites

- CMake 3.22 or higher
- Visual Studio (with C++ development tools installed)
- Git

#### Steps

1. **Open CMake GUI**
2. Set the **source code** path to the project folder
3. Set the **build directory** to a new folder
4. Click **Configure** and select the appropriate Visual Studio version
5. Click **Generate** to create Visual Studio project files
6. Open the generated `.sln` file in Visual Studio
7. Build the project by selecting **Build Solution
8. Run the executable from the `build/Debug` or `build/Release` folder

#### Command Line Alternative

You can also build directly using the terminal:

```bash
mkdir build && cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

---

### 🐧 On Linux

#### Prerequisites

- CMake 3.22 or higher
- C++20 compatible compiler
- Git

#### Install Dependencies

These libraries are required for rendering and window management:

The following commands install essential system libraries needed for the game:

- `libgl1-mesa-dev`: Provides OpenGL support for rendering graphics.
- `libX11-devel`, `libXrandr-devel`, `libXinerama-devel`, `libXcursor-devel`, `libXi-devel`: These libraries handle window creation, input events, and screen resizing using the X11 windowing system, which is standard for Linux graphical applications.

```bash
sudo apt-get install libgl1-mesa-dev       # OpenGL support
sudo dnf install libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel  # X11 windowing system
```

#### Debug Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```

#### Release Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

---

### Web Build (Using Emscripten)

```bash
cmake -DPLATFORM=Web ..
cmake --build .
```

---

### Build Options

- Debug build: `-DCMAKE_BUILD_TYPE=Debug`
- Release build: `-DCMAKE_BUILD_TYPE=Release`
- Web build (requires Emscripten): `-DPLATFORM=Web`

---

### Running the Game (After Build)

#### Asset Folder for Release Builds

In release builds, make sure to move the `assets` folder into the build folder, where the executable is located.

After building, navigate to the `build` directory and run:

```bash
./BladeAndStone2
```

On Windows:

- Find and run the executable in `build/Debug` or `build/Release` folders.
  
## Controls
- WASD - Movement
- Space - Start/Reset game

## Editing LDtk Maps
The game uses LDtk for level design. Here's how to edit maps:

### Entities
1. **Player**: Place the `Player` entity where you want them to start
2. **Enemy Spawner**: Add spawners and customize their properties in LDtk

### Tile Layers
- Place tiles in their correct layers
- Tiles in the `Ground` layer marked as `Solid` automatically handle collision

## Why Raylib and C++?
Raylib provides fast 2D graphics with a clean API, while C++ offers the performance and control needed for development.

## Assets
- Character sprites: [Link](https://kevins-moms-house.itch.io/dungeonsprites)
- Environment sprites: [Link](https://kevins-moms-house.itch.io/tundra)
- Weapon sprites: [Link](https://scracho.itch.io/bright-colored-fantasy-swords-32bit)
- Thanks to the asset creators for providing the resources used in this game.

### Template
This project was built using the [raylib-cpp-cmake-template](https://github.com/tupini07/raylib-cpp-cmake-template) as a starting point. Special thanks to tupini07 for providing this excellent template that helped jumpstart the development process.

### Wait BladeAndStone2 ?? Where is the first one?
- [BladeAndStone](https://github.com/Tlamir/BladeAndStone) - The first version of the game had a much simpler design and far fewer features compared to this.

## License
[MIT License](https://github.com/Tlamir/BladeAndStone2/blob/main/LICENSE)
