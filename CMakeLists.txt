cmake_minimum_required(VERSION 3.19)

function(RUN_CMAKE pathtocmakelists)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${pathtocmakelists})
    if (result)
        message(FATAL_ERROR "CMake step for ${pathtocmakelists} failed: ${result}")
    endif ()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${pathtocmakelists})
    if(result)
        message(FATAL_ERROR "Build step for ${pathtocmakelists} failed: ${result}")
    endif()
endfunction()
# Download and unpack library at configure time
function(SETUP_LIBRARY lib_name lib_source tag)
    # Copy the downloading CMakeLists.txt into a folder
    configure_file(CMakeLists.txt.in ${lib_name}-download/CMakeLists.txt)

    # Run it
    RUN_CMAKE(${lib_name}-download)

    # Add libary directly to our build. This defines the targets
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src
            ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-build
            EXCLUDE_FROM_ALL)

    include_directories("${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/include")
endfunction()

function(SETUP_LIBRARY_SVN lib_name lib_source cmakelistspath)
#Special for libmpg123
configure_file(CMakeLists.svn.txt.in ${lib_name}-download/CMakeLists.txt)
RUN_CMAKE(${lib_name}-download)

RUN_CMAKE(${lib_name}-src/${cmakelistspath})

    # Add libary directly to our build. This defines the targets
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/${cmakelistspath}
#        ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-build
#        EXCLUDE_FROM_ALL)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/${cmakelistspath}/src/libmpg123)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/${cmakelistspath}/src/libout123)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/${cmakelistspath}/src/libsyn123)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/${cmakelistspath}/src/libmpg123)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/src/libmpg123")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/src/libout123")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/src/libsyn123")
endfunction()
SETUP_LIBRARY_SVN(libmpg123 svn://scm.orgis.org/mpg123/trunk ports/cmake)


SETUP_LIBRARY(sdl https://github.com/SDL-mirror/SDL.git master)
#SET(SDL2_DIR ${CMAKE_CURRENT_BINARY_DIR}/sdl-bin)
SET(SDL2W_SDL2_DIR ${CMAKE_CURRENT_BINARY_DIR}/sdl-bin/CMakeFiles/Export/lib/cmake/SDL2)
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/sdl-bin/CMakeFiles/Export/lib/cmake/SDL2")
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/sdl-src")
#list(APPEND CMAKE_MODULE_PATH /path/to/your/SDL2/modules)
#find_package(SDL2 REQUIRED)
SETUP_LIBRARY(sdl_ttf https://github.com/libsdl-org/SDL_ttf.git release-2.0.15)
SETUP_LIBRARY(sdl_image https://github.com/libsdl-org/SDL_image.git FALSE)
#SETUP_LIBRARY(libmpg123 https://github.com/gypified/libmpg123.git TRUE)
#SETUP_LIBRARY(flac https://github.com/xiph/flac.git FALSE)
#SET(ANDROID ON)
#set(SUPPORT_OGG OFF CACHE BOOL "" FORCE)
# Essentially make our own CMakeLists.txt for the mixer
# Might probably break when it is updated.
SET(lib_name sdl_mixer)
SET(lib_source https://github.com/SDL-mirror/SDL_mixer.git)
configure_file(CMakeLists.txt.in sdl_mixer-download/CMakeLists.txt)
RUN_CMAKE(sdl_mixer-download)
#SETUP_LIBRARY(sdl_mixer https://github.com/SDL-mirror/SDL_mixer.git FALSE)
project(SDL_mixer C)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/include ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs)

add_library(SDL2_mixer SHARED)


target_sources(SDL2_mixer PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/effect_position.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/effects_internal.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/effect_stereoreverse.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/mixer.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/music.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/utils.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/load_aiff.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/load_voc.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_cmd.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_flac.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_fluidsynth.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_mad.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_mikmod.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_modplug.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_mpg123.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_nativemidi.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/mp3utils.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_ogg.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_opus.c
        ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_timidity.c ${CMAKE_CURRENT_BINARY_DIR}/sdl_mixer-src/src/codecs/music_wav.c)

add_definitions(-DMUSIC_MP3_MPG123)
#add_subdirectory(external/mpg123-1.25.13)
target_link_libraries(SDL2_mixer PRIVATE libmpg123.a libsyn123.a libout123.a)
target_include_directories(SDL2_mixer PUBLIC .)
target_link_libraries(SDL2_mixer PRIVATE SDL2)


# Add libary directly to our build. This defines the targets
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src
        ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-build
        EXCLUDE_FROM_ALL)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src/include")


project(SDL2Template)

set(CMAKE_CXX_STANDARD 14)

add_executable(SDL2Template main.cpp)
target_link_libraries(SDL2Template SDL2 libmpg123.a SDL2_mixer)