cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

#if(NOT ANDROID)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#if(ANDROID)
	## armeabi-v7a requires cpufeatures library
	#include(AndroidNdkModules)
	#android_ndk_import_module_cpufeatures()
#endif()

if(NOT EMSCRIPTEN)
	if(ANDROID)
		#if(NOT ANDROID_SDL2_CMAKE_DIR)
			#message(FATAL_ERROR "ANDROID_SDL2_CMAKE_DIR not set to sdl2 src path")
		#endif()
		#add_subdirectory("${ANDROID_SDL2_CMAKE_DIR}" "${CMAKE_BINARY_DIR}/android_sdl")
	else()
		find_package(SDL2 REQUIRED)
	endif()

	if(NOT ANDROID)
		add_subdirectory("external/glad")
	endif()
endif()

add_subdirectory("src")

