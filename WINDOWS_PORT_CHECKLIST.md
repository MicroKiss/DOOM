# DOOM Windows Port Checklist

Use this file to track a learning-focused port of Linux DOOM 1.10 to modern
64-bit Windows. Keep the original software renderer and game logic intact.
Replace only the platform-dependent services at first.

## Target Toolchain

- [x] Install Visual Studio 2022 with **Desktop development with C++**.
- [x] Install CMake and Ninja.
- [x] Install Git and vcpkg.
- [x] Install SDL2 for the `x64-windows` target.
- [x] Build and run a minimal SDL2 window program.


Target choices:

- Language: C
- Architecture: x86-64
- Build system: CMake
- Platform library: SDL2
- First playable target: single-player with no sound

---

## Milestone 0: Preserve a Reference

- [x] Create a Git branch for the Windows port.
- [x] Make sure the original source remains available unchanged.
- [x] Read `i_main.c` and locate the call to `D_DoomMain()`.
- [x] Read the main loop in `d_main.c`.
- [x] Identify the public functions declared by `i_system.h`, `i_video.h`,
      `i_sound.h`, and `i_net.h`.
- [x] Obtain a legal IWAD for testing, such as the shareware `doom1.wad`.
- [ ] Optionally build the original version under Linux or WSL for comparison.

**Done when:** I can describe how execution reaches the game loop and which
`I_*` functions isolate the engine from the operating system.

## Milestone 1: Create the CMake Build

- [x] Add a root `CMakeLists.txt` or one inside `linuxdoom-1.10`.
- [x] Create an executable target containing the portable engine sources.
- [x] Exclude Linux-specific implementations initially:
  - `i_video.c`
  - `i_system.c`
  - `i_sound.c`
  - `i_net.c`
- [x] Link SDL2 through vcpkg/CMake.
- [x] Keep the old `Makefile` for reference.
- [x] Compile as C rather than C++.
- [x] Enable useful MSVC warnings without treating them as errors yet.
- [x] Generate a Visual Studio or Ninja build successfully.

**Done when:** CMake configures successfully and compilation reaches missing
platform functions rather than failing in project setup.

## Milestone 2: Add Windows Platform Stubs

- [x] Create separate SDL/Windows source files for system, video, sound, and
      networking services.
- [x] Implement every required `I_*` symbol with a temporary no-op or clear
      error message.
- [x] Keep single-player network initialization functional without sockets.
- [x] Keep sound and music disabled safely.
- [x] Make `I_Error()` print its message and terminate cleanly.
- [x] Link the complete executable.

**Done when:** A Windows executable is produced and starts far enough to report
a controlled error instead of failing to link or crashing immediately.

## Milestone 3: Make the Engine 64-Bit Clean

- [x] Replace obsolete headers such as `values.h`.
- [x] Introduce `stdint.h` types where file formats require exact widths.
- [x] Confirm `byte` is 8 bits and `fixed_t` is exactly 32 bits.
- [x] Replace pointer-to-`int` casts with `intptr_t` or `uintptr_t`.
- [x] Fix pointer alignment code in `r_data.c` and `r_draw.c`.
- [x] Redesign pointer-as-integer defaults in `m_misc.c`.
- [x] Keep WAD fields and save-game fields at their original widths.
- [x] Replace unsafe unaligned integer reads with `memcpy` where needed.
- [x] Resolve errors before reducing warnings.
- [x] Record intentional warnings that still remain.

Intentional warnings after a clean x64 Debug build:

- MSVC C4113 in `info.c` (110 occurrences): the generated state table stores
      heterogeneous legacy action routines through the `actionf_t` function-pointer
      union. Preserve this behavior until the action-function API is modernized as
      a separate change.

**Done when:** All portable engine files compile for x86-64 and there are no
pointer-truncation warnings.

## Milestone 4: Implement System Services

- [x] Implement `I_GetTime()` with an SDL monotonic clock.
- [x] Convert elapsed time to DOOM's `TICRATE` of 35 tics per second.
- [x] Implement `I_WaitVBL()` with `SDL_Delay()`.
- [x] Keep zone allocation based on `malloc()`.
- [x] Implement orderly initialization and shutdown.
- [x] Ensure errors and normal exits both release SDL resources.
- [x] Print diagnostic startup messages in Debug builds.

**Verification:**

- [x] Log the tic count for a short test.
- [x] Confirm approximately 35 tics pass per real-time second.
- [x] Confirm the timer never moves backward.

**Done when:** Startup and shutdown are reliable and game time advances at the
expected rate.

## Milestone 5: Present the Framebuffer

- [x] Allocate `screens[0]` as a 320x200 8-bit indexed framebuffer.
- [x] Create an SDL window and renderer.
- [x] Create a streaming 32-bit RGBA texture.
- [x] Store all 256 colors received by `I_SetPalette()`.
- [x] Convert each indexed framebuffer pixel through the active palette.
- [x] Upload and present the converted texture in `I_FinishUpdate()`.
- [x] Implement integer scaling.
- [x] Preserve the image aspect ratio; optionally correct 320x200 to 4:3.
- [x] Handle window resizing without changing the game framebuffer size.
- [x] Avoid changing any renderer code outside the platform layer.

**Verification:**

- [x] The title screen appears.
- [x] Colors match a known screenshot or the Linux reference.
- [ ] Palette flashes work when taking damage or collecting items.
- [x] Resizing does not distort memory or crash.

**Done when:** Animated title/demo frames display with correct colors.

## Milestone 6: Add Input

- [x] Poll SDL events from `I_StartTic()`.
- [x] Translate SDL keyboard events to DOOM `KEY_*` values.
- [x] Post `ev_keydown` and `ev_keyup` with `D_PostEvent()`.
- [x] Post mouse buttons and relative movement as `ev_mouse`.
- [x] Enable relative mouse mode during gameplay.
- [x] Release input state when the window loses focus.
- [x] Convert a window-close event into a clean quit.
- [x] Verify Escape, Enter, arrows, function keys, Ctrl, Alt, and Shift.

**Verification:**

- [x] Navigate every main menu using the keyboard.
- [x] Start a game and move, turn, fire, use, and switch weapons.
- [x] Alt-tab away and back without stuck keys or mouse buttons.
- [x] Close the window without a crash.

**Done when:** The game is controllable with keyboard and mouse.

## Milestone 7: Verify WAD and File Handling

- [x] Open WADs and save files in binary mode.
- [x] Test a path containing spaces.
- [x] Test Windows path separators and relative paths.
- [x] Avoid relying on case-sensitive file names.
- [x] Add a clear error for a missing or invalid IWAD.
- [x] Keep commercial IWAD files outside source control and packages.

**Verification:**

- [x] Launch with an explicit IWAD argument.
- [ ] Start a level and reach the exit.
- [ ] Save and load a game.
- [ ] Restart after dying.
- [x] Transition between two levels.

**Done when:** A complete single-player session works without sound.

## Milestone 8: Add Sound Effects

- [x] Study how `s_sound.c` calls the `I_Sound*` interface.
- [x] Parse DOOM sound lumps into PCM samples.
- [x] Open an SDL audio device with a known output format.
- [x] Implement multiple mixing channels.
- [x] Implement volume and stereo separation.
- [x] Implement pitch changes or document a temporary omission.
- [x] Implement start, stop, parameter update, and playing-status functions.
- [x] Synchronize game-thread and audio-thread access safely.
- [x] Shut down audio without callback use-after-free errors.

**Verification:**

- [x] Fire several weapons and hear distinct effects.
- [x] Trigger multiple enemies and sounds simultaneously.
- [ ] Change sound volume from the menu.
- [ ] Play for ten minutes without pops, hangs, or crashes.

**Done when:** Overlapping positional sound effects are stable.

## Milestone 9: Add Music

- [ ] Learn the difference between DOOM MUS data, MIDI events, and PCM audio.
- [ ] Select an established MUS/MIDI solution such as FluidSynth or
      libADLMIDI.
- [ ] Implement song registration and unregistration.
- [ ] Implement play, loop, pause, resume, and stop.
- [ ] Implement music volume.
- [ ] Package any required legal sound font or document how to provide one.

**Verification:**

- [ ] Menu and level music start correctly.
- [ ] Music loops without an obvious gap.
- [ ] Pause/resume and volume controls work.
- [ ] Level transitions replace the current song correctly.

**Done when:** Music works throughout a normal single-player session.

## Milestone 10: Port Networking

- [ ] Read `d_net.c`, `doomdata.h`, and the original `i_net.c`.
- [ ] Choose Winsock or SDL_net.
- [ ] Initialize and clean up the network library correctly.
- [ ] Create nonblocking UDP sockets.
- [ ] Preserve packet field widths and byte order.
- [ ] Replace deprecated hostname lookup APIs where practical.
- [ ] Report socket errors with useful messages.
- [ ] Keep the no-network single-player path independent and reliable.

**Verification:**

- [ ] Run two local instances on different command lines.
- [ ] Complete a multiplayer game over localhost.
- [ ] Repeat between two Windows machines on a LAN.
- [ ] Disconnect one instance and confirm the other fails predictably.

**Done when:** A multiplayer session works without desynchronizing.

## Milestone 11: Regression and Stability Testing

- [ ] Run title/demo playback for at least ten minutes.
- [ ] Run `-timedemo` repeatedly and record results.
- [ ] Compare screenshots at known demo points.
- [ ] Test Debug and Release builds.
- [ ] Test windowed and fullscreen modes.
- [ ] Test repeated start/quit cycles.
- [ ] Test focus loss during gameplay and menus.
- [ ] Test save/load across executable restarts.
- [ ] Test with AddressSanitizer using Clang or supported MSVC tooling.
- [ ] Investigate every sanitizer error before packaging.

**Done when:** Core gameplay, demos, saves, input, audio, and shutdown survive
repeatable testing with no known memory errors.

## Milestone 12: Package the Port

- [ ] Produce a clean Release build from a fresh checkout.
- [ ] Include required SDL and runtime DLLs.
- [ ] Do not include commercial IWAD data.
- [ ] Document how the player supplies an IWAD.
- [ ] Retain required GPL license and source notices.
- [ ] Test the package on a Windows machine without development tools.
- [ ] Tag the first working release in Git.

**Done when:** A fresh Windows system can launch the packaged executable after
the user supplies a legal IWAD.

---

## Progress Log

Add one short entry after each work session.

| Date | Milestone | What I learned or changed | Next action |
|---|---:|---|---|
| YYYY-MM-DD | 0 | Example entry | Example next step |
| 2026-08-22 | 8 | Added callback-driven SDL sound effects with DMX lump parsing, mixing, panning, pitch, handles, and synchronized shutdown. | Complete the in-game listening and ten-minute stability checks. |

## Rules for the Port

- [ ] Change one subsystem at a time.
- [ ] Build after every small source change.
- [ ] Keep the original renderer working before adding enhancements.
- [ ] Fix root causes instead of suppressing compiler warnings blindly.
- [ ] Keep file-format widths separate from native C type widths.
- [ ] Commit each completed, working milestone separately.
- [ ] Do not begin networking until single-player audio is stable.