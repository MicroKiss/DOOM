# DOOM: Modern Windows Port

This project is a personal modernization of the original DOOM source release,
forked from [id Software's DOOM repository](https://github.com/id-Software/DOOM).
Its goal is to make the classic engine straightforward to build and run on
modern 64-bit Windows while preserving the original software renderer and game
logic. It also serves as a foundation for further improvements and experiments.


## Requirements

- Windows 10 or later (64-bit)
- Visual Studio with the **Desktop development with C++** workload
- CMake 3.20 or later
- [vcpkg](https://github.com/microsoft/vcpkg)
- A legally obtained DOOM IWAD

Install the required libraries with vcpkg:

```powershell
vcpkg install sdl2:x64-windows libadlmidi:x64-windows
```

## Build

From the repository root, configure the project with the path to your vcpkg
toolchain file:

```powershell
cmake -S . -B build -A x64 `
	-DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release --parallel
```

The executable is written to `build/Release/doom.exe`. CMake also copies the
required libADLMIDI runtime library beside the executable.

For the exact commands used by this repository's development environment, see
[README_CMAKE_COMMANDS.md](README_CMAKE_COMMANDS.md).

## Run

Pass the path to your IWAD with `-iwad`:

```powershell
.\build\Release\doom.exe -iwad "C:\path\to\DOOM1.WAD"
```

If `-iwad` is omitted, the engine searches the directory specified by
`DOOMWADDIR` and then the current directory.

## Game Data

This repository contains engine source code, not a license to distribute DOOM
game data. Commercial IWADs such as `doom.wad`, `doom2.wad`, `tnt.wad`, and
`plutonia.wad` remain copyrighted by their respective owners. Supply your own
legally obtained IWAD and do not include it in source archives or release
packages.

## License

The original DOOM source code and this derivative work are distributed under
the GNU General Public License v2. See [LICENSE.TXT](LICENSE.TXT) for the full
license text. Distributed builds must also include the license notices required
by SDL2 and libADLMIDI.