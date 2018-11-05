cmake_minimum_required(VERSION 3.12)
project(glexample C)
set(
        MINGW_INC
        "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/include"
        "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/include"
)
set(
        MINGW_LIB
        "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/lib"
        "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/lib"
)

include_directories(SYSTEM ${MINGW_INC})
link_directories(${MINGW_LIB})
set(CMAKE_C_STANDARD 99)

add_executable(glexample main.c h7_sdl.c h7_gl.c hitbox.h)
target_link_libraries(glexample mingw32 SDL2main SDL2 opengl32)