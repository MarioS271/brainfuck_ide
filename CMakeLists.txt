cmake_minimum_required(VERSION 4.2)
project(brainfuck_ide C)

set(CMAKE_C_STANDARD 23)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c src/**/*.c)
file(GLOB PDCURSES_CORE CONFIGURE_DEPENDS lib/PDCurses/pdcurses/*.c)
file(GLOB PDCURSES_BACKEND CONFIGURE_DEPENDS lib/PDCurses/wincon/*.c)

add_executable(brainfuck_ide ${SOURCES} ${PDCURSES_CORE} ${PDCURSES_BACKEND})

target_include_directories(brainfuck_ide PRIVATE lib lib/PDCurses)
target_compile_definitions(brainfuck_ide PRIVATE PDC_WIDE)
target_link_options(brainfuck_ide PRIVATE -mconsole)
