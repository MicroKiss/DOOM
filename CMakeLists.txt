cmake_minimum_required(VERSION 3.20)

project(doom_windows_port LANGUAGES CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(libADLMIDI CONFIG REQUIRED)

set(DOOM_CPP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(DOOM_CPP_SOURCES
    ${DOOM_CPP_SOURCE_DIR}/AutoMap/am_map.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/d_items.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/d_main.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/d_net.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/doomdef.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/doomstat.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/dstrings.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/info.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/tables.cpp
    ${DOOM_CPP_SOURCE_DIR}/Finale/f_finale.cpp
    ${DOOM_CPP_SOURCE_DIR}/Finale/f_wipe.cpp
    ${DOOM_CPP_SOURCE_DIR}/Game/g_game.cpp
    ${DOOM_CPP_SOURCE_DIR}/HeadsUpDisplay/hu_lib.cpp
    ${DOOM_CPP_SOURCE_DIR}/HeadsUpDisplay/hu_stuff.cpp
    ${DOOM_CPP_SOURCE_DIR}/Intermission/wi_stuff.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_bsp.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_data.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_draw.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_main.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_plane.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_segs.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_sky.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/r_things.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/i_main.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/i_main_test.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/i_net.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/i_sound.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/i_system.cpp
    ${DOOM_CPP_SOURCE_DIR}/SystemInterface/i_video.cpp
    ${DOOM_CPP_SOURCE_DIR}/Sound/sounds.cpp
    ${DOOM_CPP_SOURCE_DIR}/Sound/s_sound.cpp
    ${DOOM_CPP_SOURCE_DIR}/StatusBar/st_lib.cpp
    ${DOOM_CPP_SOURCE_DIR}/StatusBar/st_stuff.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_ceilng.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_doors.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_enemy.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_floor.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_inter.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_lights.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_map.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_maputl.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_mobj.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_plats.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_pspr.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_saveg.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_setup.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_sight.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_spec.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_switch.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_telept.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_tick.cpp
    ${DOOM_CPP_SOURCE_DIR}/PlaySimulation/p_user.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_swap.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_bbox.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_random.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_argv.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_cheat.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_fixed.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_menu.cpp
    ${DOOM_CPP_SOURCE_DIR}/Miscellaneous/m_misc.cpp
    ${DOOM_CPP_SOURCE_DIR}/ZoneMemory/z_zone.cpp
    ${DOOM_CPP_SOURCE_DIR}/Renderer/v_video.cpp
    ${DOOM_CPP_SOURCE_DIR}/Wad/w_wad.cpp

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