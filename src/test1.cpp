#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL.h>

#ifdef __EMSCRIPTEN__

#ifdef USE_OPENGL_3
#error "emscripten has no opengl 3 support, use opengl es 3 instead"
#endif

#ifndef USE_OPENGL_ES_3
#define USE_OPENGL_ES_3
#endif

#endif // __EMSCRIPTEN__

#if !defined(USE_OPENGL_3) && !defined(USE_OPENGL_ES_3)
#define USE_OPENGL_3 // default to opengl 3
#endif

#if defined(USE_OPENGL_3) && defined(USE_OPENGL_ES_3)
#error "can not have both, opengl 3 and opengl es 3"
#endif

#ifdef USE_OPENGL_3
//#include <GL/gl.h>
//#include <SDL_opengl.h>
#include <glad/glad.h>
#else
//#include <SDL_opengles3.h>
#include <SDL_opengles2.h> // bc bc
#endif

#include <iostream>

// globals
SDL_Window* g_win = nullptr;
SDL_GLContext g_gl_context;
volatile bool g_quit = false;

void quit(void) {
	g_quit = true;

#ifdef __EMSCRIPTEN__
#endif
}

void main_loop_iteration(void) {
	// events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit();
				break;
		}
	}

	// render
	SDL_GL_MakeCurrent(g_win, g_gl_context);
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(g_win);
}

int main(int, char**) {
	// setup sdl and gl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER)) {
		std::cerr << "error initializing sdl\n";
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);

#ifdef USE_OPENGL_ES_3 // opengl es 3.0 (webgl 2)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else // opengl 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

	g_win = SDL_CreateWindow("test1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
	if (!g_win) {
		std::cerr << "error opening window\n";
		return 1;
	}

	g_gl_context = SDL_GL_CreateContext(g_win);
	if (!g_gl_context) {
		std::cerr << "error creating webgl context\n";
		return 1;
	}


#ifdef USE_OPENGL_3
	if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
		std::cerr << "error loading gl (glad)\n";
		return 1;
	}
#endif

	if (SDL_GL_MakeCurrent(g_win, g_gl_context)) {
		std::cerr << "error making gl context current\n";
		return 1;
	}

	std::cout << "gl version: " << glGetString(GL_VERSION) << "\n";

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop_iteration, 0, true);
#else
	while (!g_quit) {
		main_loop_iteration();
	}

	SDL_GL_DeleteContext(g_gl_context);
	SDL_DestroyWindow(g_win);
	SDL_Quit();

	return 0;
#endif
}

