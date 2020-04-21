cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_executable (got 
    "1_BACK.CPP"
    "1_BOSS1.CPP"
    "1_BOSS21.CPP"
    "1_BOSS22.CPP"
    "1_DIALOG.CPP"
    "1_FILE.CPP"
    "1_GRP.CPP"
    "1_IMAGE.CPP"
    "1_INIT.CPP"
    "1_MAIN.CPP"
    "1_MOVE.CPP"
    "1_MOVPAT.CPP"
    "1_MUSIC.CPP"
    "1_OBJECT.CPP"
    "1_PANEL.CPP"
    "1_SBFX.CPP"
    "1_SOUND.CPP"
    "1_SCRIPT.CPP"
    "1_SHTMOV.CPP"
    "1_SHTPAT.CPP"
    "1_SPTILE.CPP"
    "fx_man.cpp"
    "g_asm.cpp"
    "mu_man.cpp"
    "pal_man.cpp"
    "res_man.cpp"
    "sb_man.cpp"
    "sdl_graphics.cpp"
    "sdl_man.cpp"
    "utility.cpp"
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(got PRIVATE SDL2::SDL2 SDL2::SDL2main)

# TODO: Add tests and install targets if needed.
