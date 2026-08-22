# CMake Commands for the Windows Port

Run these commands from the repository root (`A:\Dev\DOOM`) in PowerShell or
Command Prompt.

## Install Dependencies

```powershell
A:\Dev\Git\vcpkg\vcpkg.exe install sdl2:x64-windows libadlmidi:x64-windows
```

SDL2 provides the window, input, and audio device. libADLMIDI reads DOOM MUS
song data and synthesizes stereo PCM with its embedded OPL instrument banks, so
no external SoundFont is required.

## Configure and Generate

```powershell
cmake -S . -B build-windows-vs18-sdl -G "Visual Studio 18 2026" -A x64 -DCMAKE_TOOLCHAIN_FILE=A:/Dev/Git/vcpkg/scripts/buildsystems/vcpkg.cmake
```

This command configures the CMake project and generates Visual Studio build
files.

- `-S .` uses the current directory as the source directory containing the
  root `CMakeLists.txt`.
- `-B build-windows-vs18-sdl` writes generated files into a separate build
  directory, leaving the source tree clean.
- `-G "Visual Studio 18 2026"` selects the installed Visual Studio generator.
- `-A x64` generates a 64-bit Windows build.
- `-DCMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake` lets CMake discover SDL2 and
  libADLMIDI installed by vcpkg for the `x64-windows` triplet.

Run this command again after changing CMake configuration files.

## Build the Debug Executable

```powershell
cmake --build build-windows-vs18-sdl --config Debug --parallel
```

This command asks CMake to build the generated Visual Studio project.

- `--build build-windows-vs18-sdl` builds the project in that generated build
  directory.
- `--config Debug` selects the Debug configuration. Use `Release` instead for
  an optimized release build.
- `--parallel` allows independent source files to compile concurrently.

The configure command must complete successfully before running the build
command.

## Run with an IWAD

Pass the IWAD explicitly with `-iwad`. Quote paths containing spaces:

```powershell
.\build-windows-vs18-sdl\Debug\doom.exe -iwad ".\DOOM1.WAD"
```

Relative paths and both `/` and `\` path separators are supported. Known IWAD
file names are matched without case sensitivity. If `-iwad` is omitted, the
game searches `DOOMWADDIR` and then the current directory for a known IWAD.

Commercial IWADs such as `doom.wad`, `doomu.wad`, `doom2.wad`, `tnt.wad`, and
`plutonia.wad` are copyrighted game data. They are ignored by Git and must not
be included in source archives or release packages; each player supplies their
own legal copy.

## Music and Release Packaging

DOOM MUS lumps contain compact sequenced note and controller commands. MIDI is
the standardized event representation used by synthesizers, while PCM is the
final stream of audio sample amplitudes sent to SDL. libADLMIDI parses MUS and
synthesizes PCM directly inside the SDL audio callback.

CMake copies `ADLMIDI.dll` beside `doom.exe`. Release packages must include
that DLL and libADLMIDI's LGPL-3.0 license notice from the vcpkg package. No
SoundFont file is used or distributed.