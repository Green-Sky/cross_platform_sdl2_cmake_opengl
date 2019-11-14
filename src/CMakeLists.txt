cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(sdl_test C CXX)

if(ANDROID)
	add_library(test1 SHARED)
else()
	add_executable(test1)
endif()

target_sources(test1 PRIVATE
	./test1.cpp
)


target_include_directories(test1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(EMSCRIPTEN)
	set_target_properties(test1 PROPERTIES COMPILE_FLAGS "-s USE_SDL=2 -s USE_WEBGL2=1")
	set_target_properties(test1 PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_WEBGL2=1")
else()
	if(ANDROID)
		#target_include_directories(test1 PUBLIC "${SDL2_INCLUDE_DIR}")
		target_link_libraries(test1 SDL2main SDL2)
	else()
		target_include_directories(test1 PUBLIC "${SDL2_INCLUDE_DIR}")
		target_link_libraries(test1 ${SDL2_LIBRARY})
	endif()

	
	if(FALSE) # opengl 3
		target_link_libraries(test1 glad)
	else() # opengl es "3"
		target_compile_definitions(test1 PUBLIC "USE_OPENGL_ES_3")
		if(NOT ANDROID)
			target_link_libraries(test1 "GL") # gles ? unix?
		endif()
	endif()
endif()


