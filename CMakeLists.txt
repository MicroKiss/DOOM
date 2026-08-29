cmake_minimum_required(VERSION 3.20)

project(doom_windows_port LANGUAGES C CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(libADLMIDI CONFIG REQUIRED)

set(DOOM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/linuxdoom-1.10")
set(DOOM_CPP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(DOOM_WINDOWS_SOURCES 
    ${DOOM_SOURCE_DIR}/i_main.c
    ${DOOM_SOURCE_DIR}/i_main_test.c
    ${DOOM_SOURCE_DIR}/i_video.c
    ${DOOM_SOURCE_DIR}/i_sound.c
    ${DOOM_SOURCE_DIR}/i_system.c
    ${DOOM_SOURCE_DIR}/i_net.c
)

set(DOOM_PORTABLE_SOURCES
    doomdef.c
    doomstat.c
    dstrings.c
    tables.c
    f_finale.c
    f_wipe.c
    g_game.c
    r_bsp.c
    r_data.c
    r_draw.c
    r_main.c
    r_plane.c
    r_segs.c
    r_sky.c
    r_things.c
    w_wad.c
    wi_stuff.c
    st_lib.c
    st_stuff.c
    hu_stuff.c
    hu_lib.c
    s_sound.c
    info.c
    sounds.c
    i_main.c
)

list(TRANSFORM DOOM_PORTABLE_SOURCES PREPEND "${DOOM_SOURCE_DIR}/")

set(DOOM_CPP_SOURCES
    ${DOOM_CPP_SOURCE_DIR}/AutoMap/am_map.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/d_items.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/d_main.cpp
    ${DOOM_CPP_SOURCE_DIR}/Doom/d_net.cpp
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
    ${DOOM_CPP_SOURCE_DIR}/v_video.cpp

)

add_executable(doom ${DOOM_PORTABLE_SOURCES} ${DOOM_WINDOWS_SOURCES} ${DOOM_CPP_SOURCES})

target_include_directories(doom PRIVATE
    "${DOOM_SOURCE_DIR}"
    "${DOOM_CPP_SOURCE_DIR}"
    "${DOOM_CPP_SOURCE_DIR}/AutoMap"
    "${DOOM_CPP_SOURCE_DIR}/Doom"
    "${DOOM_CPP_SOURCE_DIR}/PlaySimulation"
    "${DOOM_CPP_SOURCE_DIR}/Miscellaneous"
    "${DOOM_CPP_SOURCE_DIR}/ZoneMemory"
)

target_link_libraries(doom PRIVATE SDL2::SDL2main SDL2::SDL2 libADLMIDI::ADLMIDI)

target_compile_definitions(doom PRIVATE
    $<$<CONFIG:Debug>:DOOM_DEBUG>
)

set_target_properties(doom PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)