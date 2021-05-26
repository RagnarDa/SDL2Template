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
function(SETUP_LIBRARY lib_name lib_source usemake)
    configure_file(CMakeLists.txt.in ${lib_name}-download/CMakeLists.txt)
    if (usemake)
        # Use make
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-download)
        if (result)
            message(FATAL_ERROR "CMake step for ${lib_name} failed: ${result}")
        endif ()
        execute_process(COMMAND ./configure
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src )
        if(result)
            message(FATAL_ERROR "configure step for ${lib_name} failed: ${result}")
        endif()
        execute_process(COMMAND make
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src )
        if(result)
            message(FATAL_ERROR "make step for ${lib_name} failed: ${result}")
        endif()
        execute_process(COMMAND make install
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}-src )
        if(result)
            message(FATAL_ERROR "make step for ${lib_name} failed: ${result}")
        endif()
    else()
        # Use Cmake
        RUN_CMAKE(${lib_name}-download)
    endif()


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


SETUP_LIBRARY(sdl https://github.com/SDL-mirror/SDL.git FALSE)
#SETUP_LIBRARY(libmpg123 https://github.com/gypified/libmpg123.git TRUE)
#SETUP_LIBRARY(flac https://github.com/xiph/flac.git FALSE)
#SET(ANDROID ON)
#set(SUPPORT_OGG OFF CACHE BOOL "" FORCE)
# Essentially make our own CMakeLists.txt for the mixer
configure_file(CMakeLists.txt.in sdl_mixer-download/CMakeLists.txt)
#SETUP_LIBRARY(sdl_mixer https://github.com/SDL-mirror/SDL_mixer.git FALSE)




project(SDL2Template)

set(CMAKE_CXX_STANDARD 14)

add_executable(SDL2Template main.cpp)
target_link_libraries(SDL2Template SDL2 libmpg123.a SDL2_mixer)