# Wukong

![gif_gameplay](https://github.com/user-attachments/assets/97be0cab-7570-455b-969c-500f251330ed)

## Description
Wukong is a top-down action-adventure game about a monkey who wants to defeat his enemies to find a treasure in a forgotten land. Use the monkey's staff to unleash attacks in order to destroy the enemies standing in your way. Charge your energy bar by attacking enemies to trigger ranged attacks or dodge assaults in critical situations. Utilize the character's full arsenal of abilities to overwhelm waves of enemies and complete your mission.

Inspiration: [The Legend of Zelda: The Minish Cap](https://en.wikipedia.org/wiki/The_Legend_of_Zelda:_The_Minish_Cap), [Dragon Ball Z: The Legacy of Goku](https://en.wikipedia.org/wiki/Dragon_Ball_Z:_The_Legacy_of_Goku)

## Implemented Features (v0.3.1)
- Wave System:
  - The player must defeat all waves of enemies
  - If the player fails, they must start over from the beginning
  - Depending on how fast you beat the game, you will receive a rank
- Pause menu to replay or exit
- Two difficulties: Easy and Hard
- Ability upgrades in the pause menu
- Different types of enemies that can use different types of weapons
- Player abilities which include:
  - Melee attacking, which also generates energy
  - Ranged attacking using small or large blasts
  - Dodging attacks

## Controls

| Key / Button | Action |
| :--- | :--- |
| **WASD** | for movement |
| **Left Mouse Button** | for melee attack |
| **Right Mouse Button** | for Blast |
| **F** | for Spirit Ball |
| **Space** | for Dodge |
| **Esc** | for the pause menu |
  
## Technical Details

The project is developed in **C++** and integrates Object-Oriented Programming concepts and design patterns:

### OOP Concepts Implemented
* **Inheritance and Polymorphism:** A base class named `Enemy` was created, from which specific enemies derive (`Enemy_Walker`, `Enemy_Ranger`, `Enemy_Rotator`). The attack system works the same way (`Attack` and `Attack_Projectile`). This allows the game to control all entities uniformly, even if they behave differently.
* **Smart Pointers & Downcasting:** Smart pointers (`std::shared_ptr`) are used so the game automatically deletes dead enemies from memory (without memory leaks). Additionally, `std::dynamic_pointer_cast` is used in `Game_Manager` to activate unique mechanics only for certain enemies (such as rotation for `Enemy_Rotator`).
* **Error Handling (Exceptions):** If a JSON file or a texture is missing, the game does not hard crash directly; instead, it throws a custom error (e.g., `AssetMissingException`), explicitly explaining the problem that occurred.
* **Templates:** A reusable class for random numbers (`Random_Value_Generator`) and generic functions (such as `RenderCollection`) were created, which can draw any list of objects on the screen, regardless of their type.

### Design Patterns & Structure
* **Builder Pattern (`Enemy_Builder`):** Allows configuring and assembling enemies in a structured way.
* **Factory Pattern (`Tool_Factory`):** Manages the automatic creation of weapons. The game only requests a specific weapon type, and this class builds it on the spot.
* **Data-Driven Architecture (JSON):** All enemy statistics, weapon details, and wave configurations (managed by `Wave_Manager`) are stored in external JSON files. This allows modifying the difficulty or adding new levels exclusively by editing text files, without needing to modify or recompile the C++ code.

## Compilation Instructions

The project is configured with CMake.

Terminal instructions:

1. Configuration step
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# or ./scripts/cmake.sh configure
```

Or on Windows with GCC using Git Bash:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# or ./scripts/cmake.sh configure -g Ninja
```

To configure with ASan, we have the option `-DUSE_ASAN=ON` (does not work on Windows with GCC):
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON
# or ./scripts/cmake.sh configure -e "-DUSE_ASAN=ON"
```

At this step, we can request to generate project files for various development environments.

2. Compilation Step
```sh
cmake --build build --config Debug --parallel 6
# or ./scripts/cmake.sh build
```
With the `parallel` option, we specify the number of files compiled in parallel.

3. Installation step (optional)
```sh
cmake --install build --config Debug --prefix install_dir
# or ./scripts/cmake.sh install
```

See also [`scripts/cmake.sh`](scripts/cmake.sh).

Note: the `build/` and `install_dir/` folders are added to the .gitignore file because they contain generated files and there is no need to version them.

## Instructions To Run The Executable

There are multiple variants:

1. From the build directory (default `build`). The executable is located at `./build/oop` after running the project compilation step (`./scripts/cmake.sh build` - step 2 above).

```sh
./build/oop
```

2. From the install directory `install_dir`. The executable is located at `./install_dir/bin/oop` after running the installation step (`./scripts/cmake.sh install` - step 3 above).

```sh
./install_dir/bin/oop
```

3. Running the program using Valgrind can be done by executing the script `./scripts/run_valgrind.sh` from the root of the project. On Windows, this script can be run using WSL (Windows Subsystem for Linux). Valgrind can be run in interactive mode using: `RUN_INTERACTIVE=true ./scripts/run_valgrind.sh`

By default, it does not run interactively, and data for `std::cin` is taken from the file `tastatura.txt`.

```sh
RUN_INTERACTIVE=true ./scripts/run_valgrind.sh
# or
./scripts/run_valgrind.sh
```

4. To run the executable using ASan, it is required that this sanitizer is enabled during the configuration step (see above). It should work on macOS and Linux. For Windows, it would only work with MSVC (not recommended).

The command is the same as in step 1 or 2. It cannot be combined with Valgrind.

```sh
./build/oop
# or
./install_dir/bin/oop
```

## Resources
- [Arial Font](https://github.com/kavin808/arial.ttf)
- [Tiny5 Font](https://fonts.google.com/specimen/Tiny5?query=pixel)
- [SFML](https://github.com/SFML/SFML/tree/3.0.2) (zlib)
- [SFML Youtube Tutorials](https://www.youtube.com/playlist?list=PL6xSOsbVA1eaJnHo_O6uB4qU8LZWzzKdo)
- [Nlohmann Json](https://github.com/nlohmann/json) used for data reading

