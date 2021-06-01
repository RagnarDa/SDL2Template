# Its assumed you are using vcpkg
# so add -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake
# vcpkg doesnt install libmpg123 so: brew install mpg123
# on windows run .\vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows SDL2-mixer[mpg123]:x64-windows


cmake_minimum_required(VERSION 3.7)
project(SDLTemplate)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

set(SOURCES main.cpp)
add_executable(SDLTemplate ${SOURCES})
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Enable testing for the project
#enable_testing()
#add_subdirectory(tst)
#:wadd_subdirectory(lib/googletest)
if (false)#(MSVC) #WIN32 #using vcpkg instead!
    MESSAGE("WINDOWS VISUAL STUDIO")
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

	include_directories(win32/include ${CMAKE_SOURCE_DIR}/include)
	target_link_libraries(SDLTemplate 
		${CMAKE_SOURCE_DIR}/win32/lib/SDL2.lib  ${CMAKE_SOURCE_DIR}/win32/lib/SDL2_image.lib
		${CMAKE_SOURCE_DIR}/win32/lib/SDL2_ttf.lib)
	#	${CMAKE_SOURCE_DIR}/lib/GoogleMock.lib ${CMAKE_SOURCE_DIR}/lib/GoogleTest.lib)
	add_custom_command(TARGET SDLTemplate POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/win32/bin"
        $<TARGET_FILE_DIR:SDLTemplate>)
	add_custom_command(TARGET SDLTemplate POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        $<TARGET_FILE_DIR:SDLTemplate>/res)
	
	# warning level 4 and all warnings as errors
	add_compile_options(/W4 /WX)
else()
    include_directories(include)
    find_package(SDL2 CONFIG REQUIRED)

    #for finding FindSDL2_image.cmake https://trenki2.github.io/blog/2017/07/04/using-sdl2-image-with-cmake/
    #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

    find_package(SDL2-image REQUIRED)
	find_package(sdl2-ttf REQUIRED)
	find_package(sdl2-mixer REQUIRED)
	#find_package(mpg123 CONFIG REQUIRED)
    #include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
	set(libraries SDL2::SDL2main SDL2::SDL2-static SDL2::SDL2_image SDL2::SDL2_ttf SDL2::SDL2_mixer )
    target_link_libraries(SDLTemplate ${libraries})# ${CMAKE_SOURCE_DIR}/lib/libgmock.a ${CMAKE_SOURCE_DIR}/lib/libgtest.a)
	target_link_libraries(${CMAKE_PROJECT_NAME}_lib ${libraries})
	
	# lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)

	# In launch.vs.json set  "currentDir": "${workspaceRoot}" in configurations to make VS launch with root dir as cwd
	if (MSVC AND FALSE)
		add_custom_command(TARGET SDLTemplate POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/win32/bin/mixer"
        $<TARGET_FILE_DIR:SDLTemplate>)	
		# VC has another 
	#	add_custom_command(TARGET SDLTemplate POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_directory
     #   "${PROJECT_SOURCE_DIR}/res"
    #    $<TARGET_FILE_DIR:SDLTemplate>/res)
	endif()
ENDIF()

