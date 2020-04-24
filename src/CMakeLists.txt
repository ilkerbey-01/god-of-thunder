cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

add_executable (got 
    "BACK.CPP"
    "BOSS1.CPP"
    "BOSS21.CPP"
    "BOSS22.CPP"
    "DIALOG.CPP"
    "FILE.CPP"
    "GRP.CPP"
    "IMAGE.CPP"
    "INIT.CPP"
    "MAIN.CPP"
    "MOVE.CPP"
    "MOVPAT.CPP"
    "MUSIC.CPP"
    "OBJECT.CPP"
    "PANEL.CPP"
    "SBFX.CPP"
    "SOUND.CPP"
    "SCRIPT.CPP"
    "SHTMOV.CPP"
    "SHTPAT.CPP"
    "SPTILE.CPP"
    "fx_man.cpp"
    "g_asm.cpp"
    "mu_man.cpp"
    "pal_man.cpp"
    "res_man.cpp"
    "sb_man.cpp"
    "sdl_graphics.cpp"
    "sdl_man.cpp"
    "utility.cpp"
    "win_man.cpp"
)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(got PRIVATE SDL2::SDL2 SDL2::SDL2main)

# TODO: Add tests and install targets if needed.
