cmake_minimum_required(VERSION 3.20)

project(doom_windows_port LANGUAGES C)

find_package(SDL2 CONFIG REQUIRED)

set(DOOM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/linuxdoom-1.10")

set(DOOM_WINDOWS_SOURCES 
    ${DOOM_SOURCE_DIR}/i_main.c
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
    d_main.c
    d_net.c
    d_items.c
    g_game.c
    m_menu.c
    m_misc.c
    m_argv.c
    m_bbox.c
    m_fixed.c
    m_swap.c
    m_cheat.c
    m_random.c
    am_map.c
    p_ceilng.c
    p_doors.c
    p_enemy.c
    p_floor.c
    p_inter.c
    p_lights.c
    p_map.c
    p_maputl.c
    p_plats.c
    p_pspr.c
    p_setup.c
    p_sight.c
    p_spec.c
    p_switch.c
    p_mobj.c
    p_telept.c
    p_tick.c
    p_saveg.c
    p_user.c
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
    v_video.c
    st_lib.c
    st_stuff.c
    hu_stuff.c
    hu_lib.c
    s_sound.c
    z_zone.c
    info.c
    sounds.c
    i_main.c
)

list(TRANSFORM DOOM_PORTABLE_SOURCES PREPEND "${DOOM_SOURCE_DIR}/")

add_executable(doom ${DOOM_PORTABLE_SOURCES} ${DOOM_WINDOWS_SOURCES})

target_include_directories(doom PRIVATE "${DOOM_SOURCE_DIR}")

target_link_libraries(doom PRIVATE SDL2::SDL2main SDL2::SDL2)

target_compile_definitions(doom PRIVATE
    $<$<CONFIG:Debug>:DOOM_DEBUG>
)

set_target_properties(doom PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)