cmake_minimum_required(VERSION 3.16)

project(lab5 LANGUAGES C ASM ASM_NASM)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        src/*.c
        src/*.h
        include/*.h
        src/*.asm
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/user/lab5/)

add_executable(lab5 ${sources})
target_link_libraries(lab5 m)
target_include_directories(lab5 PRIVATE src include)
