cmake_minimum_required(VERSION 3.20)

project(doom_windows_port LANGUAGES CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(libADLMIDI CONFIG REQUIRED)

set(DOOM_CPP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(DOOM_CPP_SOURCES
    ${DOOM_CPP_SOURCE_DIR}/AutoMap/map.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/items.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/main.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/net.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/doomdef.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/doomstat.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/dstrings.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/info.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/tables.cpp
    ${DOOM_CPP_SOURCE_DIR}/Finale/finale.cpp
    ${DOOM_CPP_SOURCE_DIR}/Finale/wipe.cpp
    ${DOOM_CPP_SOURCE_DIR}/Game/game.cpp
    ${DOOM_CPP_SOURCE_DIR}/HeadsUpDisplay/lib.cpp
    ${DOOM_CPP_SOURCE_DIR}/HeadsUpDisplay/stuff.cpp
    ${DOOM_CPP_SOURCE_DIR}/Intermission/stuff.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/bsp.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/data.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/draw.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/main.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/plane.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/segs.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/sky.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/things.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/main.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/main_test.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/net.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/sound.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/system.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/video.cpp
    ${DOOM_CPP_SOURCE_DIR}/Sound/sounds.cpp
    ${DOOM_CPP_SOURCE_DIR}/Sound/sound.cpp
    ${DOOM_CPP_SOURCE_DIR}/StatusBar/lib.cpp
    ${DOOM_CPP_SOURCE_DIR}/StatusBar/stuff.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/ceilng.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/doors.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/enemy.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/floor.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/inter.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/lights.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/map.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/maputl.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/mobj.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/plats.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/pspr.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/saveg.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/setup.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/sight.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/spec.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/switch.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/telept.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/tick.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/user.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/swap.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/bbox.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/random.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/argv.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/cheat.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/fixed.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/menu.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/misc.cpp
    ${DOOM_CPP_SOURCE_DIR}/ZoneMemory/zone.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/video.cpp
    ${DOOM_CPP_SOURCE_DIR}/Wad/wad.cpp

)

add_executable(doom ${DOOM_CPP_SOURCES})

target_include_directories(doom PRIVATE
    "${DOOM_CPP_SOURCE_DIR}"
    "${DOOM_CPP_SOURCE_DIR}/AutoMap"
    "${DOOM_CPP_SOURCE_DIR}/Doom"
    "${DOOM_CPP_SOURCE_DIR}/Finale"
    "${DOOM_CPP_SOURCE_DIR}/Game"
    "${DOOM_CPP_SOURCE_DIR}/HeadsUpDisplay"
    "${DOOM_CPP_SOURCE_DIR}/Intermission"
    "${DOOM_CPP_SOURCE_DIR}/Renderer"
    "${DOOM_CPP_SOURCE_DIR}/SystemInterface"
    "${DOOM_CPP_SOURCE_DIR}/Sound"
    "${DOOM_CPP_SOURCE_DIR}/StatusBar"
    "${DOOM_CPP_SOURCE_DIR}/PlaySimulation"
    "${DOOM_CPP_SOURCE_DIR}/Miscellaneous"
    "${DOOM_CPP_SOURCE_DIR}/ZoneMemory"
    "${DOOM_CPP_SOURCE_DIR}/Wad"
)

target_link_libraries(doom PRIVATE SDL2::SDL2main SDL2::SDL2 libADLMIDI::ADLMIDI)

target_compile_definitions(doom PRIVATE
    $<$<CONFIG:Debug>:DOOM_DEBUG>
)

set_target_properties(doom PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)