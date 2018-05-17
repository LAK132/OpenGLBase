#include "main.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLAK_MULTITHREAD
#define GLAK_HANDLE_MAIN
#define GLAK_IMPLEMENTATION
#include <glak.hpp>

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
    if(ImGui::TreeNode("tinyobjloader"))
    {
        ImGui::Text(R"(https://github.com/syoyo/tinyobjloader

The MIT License (MIT)

Copyright (c) 2012-2016 Syoyo Fujita and many contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.)");
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

Copyright (c) 2018 Lucas Kleiss (LAK132)

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

///
/// loop()
/// Called every loop
///
void update(glakLoopData* ld, double deltaTime)
{
    userData_t* ud = (userData_t*)ld->userData;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&event);
        if (event.type == SDL_QUIT) ld->run = false;
    }
    ImGui_ImplSdlGL3_NewFrame(ld->window);

    // static bool is not reset every time loop() is called, effectively global variable but cannot be accessed outside of the loop() scope
    static bool rightMenuOpen = true;
    // Check if ImGui wants to use the mouse
    if(!ud->io->WantCaptureMouse)
    {
        // If ImGui is not using the mouse, check if the right button has been clicked
        if(ud->io->MouseClicked[1])
        {
            cout << "right mouse clicked\n";
            // Toggle rightMenuOpen
            rightMenuOpen = !rightMenuOpen;
        }
    }
    // Check if ImGui wants to use the keyboard
    if(!ud->io->WantCaptureKeyboard)
    {

    }
    
    // If rightMenuOpen draw a small menu
    if(rightMenuOpen)
    {
        // Set the window pos to the mouse pos
        ImGui::SetNextWindowPos(ImVec2(ud->io->MouseClickedPos[1][0] - 2, ud->io->MouseClickedPos[1][1] - 2), ImGuiCond_Appearing);
        // Draw a window with no boarder or menu
        if(ImGui::Begin("Right Click Menu", &rightMenuOpen, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Delta Time %f", deltaTime);
            // Draw the library credits
            credits();
        }
        // End the window
        ImGui::End();
    }
    ImGui::Render();
}


///
/// draw()
/// Called every loop
///
void draw(glakLoopData* ld, double deltaTime)
{
    userData_t* ud = (userData_t*)ld->userData;
    glViewport(0, 0, (int)ud->io->DisplaySize.x, (int)ud->io->DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our object
    ud->obj.draw();

    ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(ld->window);
}

///
/// init()
/// This will only be run once (when the application starts)
///
void init(glakLoopData* ld)
{
    userData_t* ud = new userData_t();
    // Setup SDL
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        throw exception();
    }

    // Setup Window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    ld->window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    ld->glContext = SDL_GL_CreateContext(ld->window);
    if (SDL_GL_SetSwapInterval(-1) == -1) // adaptive vsync
    {
        SDL_GL_SetSwapInterval(1); // standard vsync
    }

    // Initialise gl3w
    gl3wInit();

    // Initialise ImGui
    ImGui::CreateContext();
    ud->io = &ImGui::GetIO();
    ImGui_ImplSdlGL3_Init(ld->window);
    ImGui::StyleColorsDark();
    ud->style = &ImGui::GetStyle();
    ud->style->WindowRounding = 0;

    // Load a shader from file (doesn't require recompiling to change shader but less portable)
    // shader.init(glakReadFile("shaders/vshader.glsl"), glakReadFile("shaders/fshader.glsl"));

    // Create a shader (requires recompiling to change shader but more portable)
    shared_ptr<glakShader> shader = make_shared<glakShader>(glakShader(
R"(
#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

out vec4 fColor;

void main()
{
    gl_Position = vPosition;
    fColor = vColor;
})",
R"(
#version 330 core

in vec4 fColor;

out vec4 pColor;

void main() 
{ 
    pColor = fColor;
})"));

    for(auto it = shader->attributes.begin(); it != shader->attributes.end(); it++)
    {
        DEBUG << it->first << endl;
    }

    // Enable OpenGL Z buffer
    glEnable(GL_DEPTH_TEST);

    // Add a shader to the object
    ud->obj.shader.resize(1);
    ud->obj.shader[0] = shader;

    // Add a mesh to the object
    ud->obj.mesh.resize(1);
    ud->obj.mesh[0].material = 0;

    // Set up tinyobj to load a model
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shape;
    vector<tinyobj::material_t> material;

    // Try and open "teapot.obj"
    string loaderror;
    ifstream objfile;
    const char* objdir = "teapot.obj";
    objfile.open(objdir);

    if (objfile.is_open())
    {
        // If we managed to open the model, load it onto a glakObject
        tinyobj::LoadObj(&attrib, &shape, &material, &loaderror, (istream*)&objfile);
        objfile.close();

        // Copy vertex information
        glakMeshElement& vpos = ud->obj.mesh[0].elements["vPosition"];
        vpos.data = shared_ptr<void>(new glm::vec4[attrib.vertices.size()/3], std::default_delete<glm::vec4[]>());
        glm::vec4* vposptr = (glm::vec4*)vpos.data.get();
        for(size_t i = 0, j = 0; i < ud->obj.mesh[0].elements.size(); i++)
        {
            vposptr[i].x = attrib.vertices[j++]/10.0f;
            vposptr[i].y = attrib.vertices[j++]/10.0f;
            vposptr[i].z = attrib.vertices[j++]/10.0f;
        }
        vpos.size = sizeof(glm::vec4) * attrib.vertices.size()/3;
        vpos.normalized = false;
        vpos.stride = 0;

        // Copy index information
        ud->obj.mesh[0].index.resize(shape[0].mesh.indices.size());
        for(size_t i = 0; i < ud->obj.mesh[0].index.size(); i++)
        {
            ud->obj.mesh[0].index[i] = shape[0].mesh.indices[i].vertex_index;
        }
    }
    else
    {
        // If we couldn't find teapot.obj, create a flat colored plane
        cout << "Error opening OBJ file " << endl;

        // Add vertices
        glakMeshElement& vpos = ud->obj.mesh[0].elements["vPosition"];
        vpos.data = shared_ptr<void>(new glm::vec4[4], default_delete<glm::vec4[]>());
        glm::vec4* vposp = (glm::vec4*)vpos.data.get();
        vposp[0] = {-0.7f, -0.7f, 0.0f, 1.0f};
        vposp[1] = {-0.7f,  0.7f, 0.0f, 1.0f};
        vposp[2] = { 0.7f, -0.7f, 0.0f, 1.0f};
        vposp[3] = { 0.7f,  0.7f, 0.0f, 1.0f};
        vpos.normalized = false;
        vpos.size = sizeof(glm::vec4[4]);
        vpos.stride = 0;

        // Add colors
        glakMeshElement& vcol = ud->obj.mesh[0].elements["vColor"];
        vcol.data = shared_ptr<void>(new glm::vec4[4], default_delete<glm::vec4[]>());
        glm::vec4* vcolp = (glm::vec4*)vcol.data.get();
        vcolp[0] = {1.0f, 0.0f, 0.0f, 1.0f};
        vcolp[1] = {0.0f, 1.0f, 0.0f, 1.0f};
        vcolp[2] = {0.0f, 0.0f, 1.0f, 1.0f};
        vcolp[3] = {1.0f, 1.0f, 1.0f, 1.0f};
        vcol.normalized = false;
        vcol.size = sizeof(glm::vec4[4]);
        vcol.stride = 0;

        ud->obj.mesh[0].index = {0, 1, 2, 1, 2, 3};
    }

    glViewport(0, 0, 500, 500);
    glClearColor(ud->clearCol[0], ud->clearCol[1], ud->clearCol[2], ud->clearCol[3]);

    ud->obj.updateBuffer();

    ld->userData = (void*)ud;
}

///
/// destroy()
/// Called only once (at application shutdown)
///
void destroy(glakLoopData* ld)
{
    userData_t* ud = (userData_t*)ld->userData;
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(ld->glContext);
    SDL_DestroyWindow(ld->window);

    delete ud;

    SDL_Quit();
}