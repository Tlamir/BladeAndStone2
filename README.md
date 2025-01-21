# BladeAndStone2

A top-down roguelike game built in C++. The game showcases integration between LDtk level editor, Box2D physics, and Raylib graphics library, with a focus on automated systems that handle collisions and enemy spawning directly from LDtk maps.

[Download Latest Release](https://github.com/Tlamir/BladeAndStone2/releases/tag/Windows)

## Features

- Top-down roguelike gameplay with automatic combat
- Level design powered by LDtk with automatic collision handling
- Physics-based movement and collisions using Box2D
- Dynamic enemy spawning system
- Dual Weapon System: Combine sword combat with a bullet-based magic system
- Debug visualization options

## Built With

- C++20
- [Raylib](https://www.raylib.com/) (v5.0) - Graphics library
- [Box2D](https://box2d.org/) (v2.4.1) - Physics engine
- [LDtkLoader](https://github.com/Madour/LDtkLoader) (v1.5.3.1) - Level loading

## Project Structure

```
├── sources/
│   ├── entities/       # Game entities
│   ├── physics/       # Box2D integration
│   ├── scenes/        # Scene management
│   └── utils/         # Utilities
├── assets/            # Game resources
└── CMakeLists.txt    # Build configuration
```

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

## Assets
- Character sprites: [Link](https://kevins-moms-house.itch.io/dungeonsprites)
- Weapon sprites: [Link](https://kevins-moms-house.itch.io/tundra)
- Environment sprites: [Link](https://scracho.itch.io/bright-colored-fantasy-swords-32bit)
- Thanks to the asset creators for providing the resources used in this game. 😎

## License
[MIT License](https://github.com/Tlamir/BladeAndStone2/blob/main/LICENSE)

## Related Projects

### Libraries
- [Raylib](https://github.com/raysan5/raylib)
- [Box2D](https://github.com/erincatto/box2d)
- [LDtk](https://ldtk.io/)
- [LDtkLoader](https://github.com/Madour/LDtkLoader)

### Wait BladeAndStone2 ?? Where is first one ?
- [BladeAndStone](https://github.com/Tlamir/BladeAndStone) - First version of the game had a much simpler design and far fewer features compared to this.
