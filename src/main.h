#include <stdint.h>
#include <iostream>
using std::istream;
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include <tiny_obj_loader.h>

#define GLAK_MULTITHREAD
#define GLAK_HANDLE_MAIN
#include <glak.hpp>
using std::default_delete;

#define APP_NAME "OpenGL Demo Application"

#ifndef MAIN_H
#define MAIN_H

struct model_t
{
    glakTransform transform;
    shared_ptr<glakShader> shader;
    glakMesh mesh;
    string modelUniformName = "model";
    void updateBuffer();
    void draw();
};

struct userData_t
{
    float clearCol[4] = {0.0f, 0.3125f, 0.3125f, 1.0f};

    model_t obj;

    ImGuiIO* io = nullptr;
    ImGuiStyle* style = nullptr;
};

#endif