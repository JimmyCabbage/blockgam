add_executable(blockgam-e
               main.c
               g_board.c g_board.h
               g_main.c g_main.h
               g_piece.c g_piece.h
               g_ticktimer.c g_ticktimer.h
               m_menu.c m_menu.h
               v_video.c v_video.h g_board.h g_board.c)

target_link_libraries(blockgam-e PkgConfig::SDL2 PkgConfig::SDL2TTF)

if(MSVC)
    target_compile_options(blockgam-e PRIVATE /W4)
else()
    target_compile_options(blockgam-e PRIVATE -Wall -Wextra -Wpedantic)
endif()
