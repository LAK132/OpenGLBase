#include <stdint.h>
#include <iostream>
using std::istream;
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <string>
using std::string;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <cassert>

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include <imguisplitter.hpp>

using std::default_delete;

#define APP_NAME "OpenGL Demo Application"

#include <lak/image.hpp>
#include <lak/texture.hpp>
#include <lak/pnm.hpp>
#include <lak/stream_util.hpp>
#include <lak/space.hpp>
#define LAK_GRAPHICS_IMPLEM
#include <lak/graphics.hpp>

#ifndef MAIN_H
#define MAIN_H

struct model_t : public lak::referenceFrame_t
{
    vector<lak::mesh_t> mesh;
    string modelUniformName = "model";
    void update() { for(auto it = mesh.begin(); it != mesh.end(); it++) it->update(); }
    void draw() { for(auto it = mesh.begin(); it != mesh.end(); it++) it->draw(); }
};

struct light_t : public lak::referenceFrame_t
{
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct camera_t : public lak::referenceFrame_t
{
    glm::mat4 projection;  
};

struct scene_t
{
    shared_ptr<lak::referenceFrame_t> world = make_shared<lak::referenceFrame_t>();
    shared_ptr<lak::referenceFrame_t> cameraBoom = make_shared<lak::referenceFrame_t>();
    shared_ptr<camera_t> camera = make_shared<camera_t>();
    shared_ptr<model_t> obj = make_shared<model_t>();
    shared_ptr<light_t> light1 = make_shared<light_t>();
    shared_ptr<light_t> light2 = make_shared<light_t>();
    shared_ptr<light_t> light3 = make_shared<light_t>();
};

struct userData_t
{
    float clearCol[4] = {0.0f, 0.3125f, 0.3125f, 1.0f};

    scene_t scene;
    
    uint32_t screenw;
    uint32_t screenh;

    ImGuiIO* io = nullptr;
    ImGuiStyle* style = nullptr;

    shared_ptr<lak::imageRGBA8_t> image;
    shared_ptr<lak::texture_t> texture;
    shared_ptr<lak::shader_t> shader;
};

#define LAK_MAIN_MULTITHREAD
#define LAK_MAIN_SDL
#define LAK_MAIN_USERDATATYPE userData_t
#include <lak/main.hpp>

void credits()
{
    ImGui::PushID("Credits");
    if(ImGui::TreeNode("ImGui"))
    {
        ImGui::Text(R"(https://github.com/ocornut/imgui

The MIT License (MIT)

Copyright (c) 2014-2018 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("glm"))
    {
        ImGui::Text(R"(https://github.com/g-truc/glm

The Happy Bunny License (Modified MIT License)

Copyright (c) 2005 - 2016 G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions
of the Software.

Restrictions: By making use of the Software for military purposes, you choose to make a Bunny unhappy.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.)");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("glak"))
    {
        ImGui::Text(R"(https://github.com/LAK132/glak
        
MIT License

Copyright (c) 2018 LAK132

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("gl3w"))
    {
        ImGui::Text("https://github.com/skaslev/gl3w");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("SDL2"))
    {
        ImGui::Text("https://www.libsdl.org/");
        ImGui::TreePop();
    }
    ImGui::PopID();
}

#endif