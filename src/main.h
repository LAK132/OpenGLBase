#include <vector>
using std::vector;
#include "glak.h"
#include "imgui_impl_glfw_gl3.h"

#define APP_NAME "OpenGL Application"

#ifndef MAIN_H
#define MAIN_H

extern GLFWwindow* window;
extern ImGuiIO* io;
extern ImVec4 clearCol;
extern vector<GLuint> shaders;

void init();
void draw();
void loop();
void destroy();

#endif