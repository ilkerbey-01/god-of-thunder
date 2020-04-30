cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

add_executable (got 
    "BACK.C"
    "BOSS1.C"
    "BOSS21.C"
    "BOSS22.C"
    "DIALOG.C"
    "FILE.C"
    "GRP.C"
    "IMAGE.C"
    "INIT.C"
    "MAIN.C"
    "MOVE.C"
    "MOVPAT.C"
    "MUSIC.C"
    "OBJECT.C"
    "PANEL.C"
    "RES_ENCO.C"
    "RES_FALL.C"
    "RES_LEN.C"
    "RES_FIND.C"
    "RES_INIT.C"
    "RES_READ.C"
    "SBFX.C"
    "SOUND.C"
    "SCRIPT.C"
    "SHTMOV.C"
    "SHTPAT.C"
    "SPTILE.C"
    "fx_man.C"
    "g_asm.C"
    "mu_man.C"
    "pal_man.C"
    "sb_man.C"
    "sdl_graphics.C"
    "sdl_man.C"
    "utility.C"
    "win_man.C"
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(got PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(sdl2-mixer CONFIG REQUIRED)
target_link_libraries(got PRIVATE SDL2::SDL2_mixer)

# TODO: Add tests and install targets if needed.
