cmake_minimum_required(VERSION 3.20)
project(metalslug VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# The neogeorecomp toolkit, checked out next to this repo.
set(NEOGEORECOMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../neogeorecomp CACHE PATH "neogeorecomp checkout")
add_subdirectory(${NEOGEORECOMP_DIR} neogeorecomp_build)

# Your ROM files (MAME set "mslug" plus the "neogeo" system ROMs, unzipped).
# Without it the player builds interpreter-only.
set(MSLUG_ROM_DIR "" CACHE PATH "Directory with your Metal Slug and Neo Geo BIOS ROM files")
# Optional: entry points a previous run found at runtime (--dump-misses).
set(MSLUG_ENTRIES "${CMAKE_BINARY_DIR}/entries.txt" CACHE FILEPATH "Extra entry points for the recompiler")


# Stage 1: the generator.
add_executable(mslug_recomp src/recomp_main.c src/game.c)
target_link_libraries(mslug_recomp PRIVATE m68krecomp)
target_include_directories(mslug_recomp PRIVATE src)

# Stage 2: the player, with the generated C when ROMs are available.
set(GAME_SOURCES src/main.c)
set(GEN_DIR ${CMAKE_BINARY_DIR}/generated)
set(GEN_FILES 16)
if(MSLUG_ROM_DIR)
    set(GEN_SOURCES ${GEN_DIR}/recomp_table.c)
    math(EXPR last "${GEN_FILES} - 1")
    foreach(i RANGE ${last})
        if(i LESS 10)
            list(APPEND GEN_SOURCES ${GEN_DIR}/recomp_0${i}.c)
        else()
            list(APPEND GEN_SOURCES ${GEN_DIR}/recomp_${i}.c)
        endif()
    endforeach()
    set(entries_arg "")
    set(entries_dep "")
    if(EXISTS ${MSLUG_ENTRIES})
        set(entries_arg --entries ${MSLUG_ENTRIES})
        set(entries_dep ${MSLUG_ENTRIES})
    endif()
    file(MAKE_DIRECTORY ${GEN_DIR})
    add_custom_command(
        OUTPUT ${GEN_SOURCES} ${GEN_DIR}/recomp_common.h
        COMMAND mslug_recomp --rom-path ${MSLUG_ROM_DIR} --out ${GEN_DIR} --files ${GEN_FILES} ${entries_arg}
        DEPENDS mslug_recomp ${entries_dep}
        COMMENT "Recompiling Metal Slug from ${MSLUG_ROM_DIR}")
    list(APPEND GAME_SOURCES ${GEN_SOURCES})
endif()

add_executable(metalslug ${GAME_SOURCES} src/game.c)
target_link_libraries(metalslug PRIVATE neogeorecomp)
target_include_directories(metalslug PRIVATE src ${GEN_DIR})
if(MSLUG_ROM_DIR)
    target_compile_definitions(metalslug PRIVATE MSLUG_HAVE_RECOMP)
    if(MSVC)
        # Generated routines are large; /bigobj lifts the section limit.
        set_source_files_properties(${GEN_SOURCES} PROPERTIES COMPILE_OPTIONS "/bigobj")
    endif()
endif()
if(MSVC)
    target_compile_definitions(metalslug PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(mslug_recomp PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
