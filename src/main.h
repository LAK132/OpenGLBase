#include <stdint.h>
#include <vector>
using std::vector;
#include "glak.h"
#include "imgui_impl_sdl_gl3.h"

#define APP_NAME "OpenGL Application"
#define SDL_MAIN_HANDLED

#ifndef MAIN_H
#define MAIN_H

extern SDL_Window* window;
extern SDL_GLContext glContext;
extern float clearCol[4];
extern glakShader shader;
extern glakBuffer vtxObj;
extern ImGuiIO* io;

void init();
void draw();
void loop(SDL_Event* event);
void destroy();

#endif