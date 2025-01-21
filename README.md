# Blade And Stone 2

A top-down roguelike game built in C++. This project demonstrates the integration of modern C++ techniques with libraries like Raylib, Box2D, and LDtk. The main goal is to showcase the versatility of C++ in game development by creating a efficient, and extensible game engine capable of handling physics, rendering, and automated systems.

![BladeAndStone2Image](https://i.imgur.com/Fh7ytZA.png)

## Platforms & Downloads

### 🎮 Play Now
- [Play on Itch.io](https://tlamir.itch.io/blade-and-stone-2) - Play in browser or download
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

- **C++20**: Modern features like constexpr, ranges, and smart pointers enhance performance and code clarity.
- **[Raylib](https://www.raylib.com/)** (v5.0): Lightweight graphics library for 2D rendering.
- **[Box2D](https://box2d.org/)** (v2.4.1): Physics engine for realistic collisions and interactions.
- **[LDtkLoader](https://github.com/Madour/LDtkLoader)** (v1.5.3.1): Seamless integration with the LDtk level editor.


## Project Structure

```
├── sources/
│   ├── entities/       # Game entities
│   ├── physics/       # Box2D integration
│   ├── scenes/        # Scene management
│   ├── utils/         # Utilities
│   ├── Constants.hpp  # Game constants (camera resolution, gameplay variables)
├── assets/            # Game resources
└── CMakeLists.txt    # Build configuration
```

### Key File: Constants.hpp
The `Constants.hpp` file is the central hub for game configurations, offering developers flexibility to fine-tune gameplay mechanics and technical settings:

- **Resolution and Scaling**: Control world dimensions (`WorldWidth`, `WorldHeight`) and camera zoom levels.
- **Gameplay Balance**: Adjust player speed, health, and combat variables (e.g., `playerSpeed`, `swordDamage`).
- **Weapon Customization**: Define weapon behavior with variables like attack speed, reload time, and damage for both sword and magic systems.
- **Debugging Tools**: Enable or disable collision visualization with `debugModeCollision`.

By centralizing these configurations, developers can rapidly prototype and iterate on gameplay mechanics.

## Building

### Prerequisites
- CMake 3.22 or higher
- C++20 compatible compiler
- Git

### Build Steps
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Build Options
- Debug build: `-DCMAKE_BUILD_TYPE=Debug`
- Release build: `-DCMAKE_BUILD_TYPE=Release`
- Web build (requires Emscripten): `-DPLATFORM=Web`

## Controls

- WASD - Movement
- Space - Start/Reset game

## Editing LDtk Maps

The game relies on LDtk maps for level design, including automatic collision and entity management. To edit the maps effectively, follow these guidelines:

### Entities

1. **Player**:  
   - Set the player's starting position by placing the `Player` entity in the desired location on the map.  

2. **Enemy Spawner**:  
   - Adjust the spawn rate, number, and type of enemies by modifying the properties of the `EnemySpawner` entity directly in LDtk. These properties are customizable per spawner.

### Tile Layers

- Ensure tiles are placed correctly within layers.
- Collision is automatically handled for tiles in the `Ground` layer marked with the `Solid` enum. No extra configuration is needed for collision behavior.

### Saving and Testing

- Save your changes in LDtk and export the project to ensure the updated map is loaded correctly into the game.
- Launch the game to test the map and verify the placement of entities and collision behavior.

## Why Raylib and C++?

Raylib provides an intuitive framework for 2D graphics while remaining lightweight and fast, making it an excellent choice for a C++ project focused on efficiency. C++'s performance and control over system resources allow for:

- Precise memory management
- Advanced object-oriented programming patterns
- Seamless integration with external libraries like Box2D and LDtk

This project is an example of how C++ can be used to build a complex system with clear abstractions and high performance, demonstrating skills in game engine development and modern programming practices.

## Assets
- Character sprites: [Link](https://kevins-moms-house.itch.io/dungeonsprites)
- Weapon sprites: [Link](https://kevins-moms-house.itch.io/tundra)
- Environment sprites: [Link](https://scracho.itch.io/bright-colored-fantasy-swords-32bit)
- Thanks to the asset creators for providing the resources used in this game. 😎

## Related Projects

### Libraries
- [Raylib](https://github.com/raysan5/raylib)
- [Box2D](https://github.com/erincatto/box2d)
- [LDtk](https://ldtk.io/)
- [LDtkLoader](https://github.com/Madour/LDtkLoader)

### Wait BladeAndStone2 ?? Where is the first one?
- [BladeAndStone](https://github.com/Tlamir/BladeAndStone) - The first version of the game had a much simpler design and far fewer features compared to this.

## License
[MIT License](https://github.com/Tlamir/BladeAndStone2/blob/main/LICENSE)

