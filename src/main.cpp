#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "main.h"

// SDL globals
SDL_Window* window = nullptr;
SDL_GLContext glContext;
float clearCol[4] = {0.0f, 0.3125f, 0.3125f, 1.00f};

// glak globals
glakShader shader;
glakObject obj;

// ImGui globals
ImGuiIO* io = nullptr;
ImGuiStyle* style = nullptr;

///
/// loop()
/// Called every loop
///
void loop()
{
    // static bool is not reset every time loop() is called, effectively global variable but cannot be accessed outside of the loop() scope
    static bool rightMenuOpen = false;
    // Check if ImGui wants to use the mouse
    if(!io->WantCaptureMouse)
    {
        // If ImGui is not using the mouse, check if the right button has been clicked
        if(io->MouseClicked[1])
        {
            cout << "Mouse 1 clicked" << endl;
            // Toggle rightMenuOpen
            rightMenuOpen = !rightMenuOpen;
        }
    }
    // Check if ImGui wants to use the keyboard
    if(!io->WantCaptureKeyboard)
    {

    }

    // If rightMenuOpen draw a small menu
    if(rightMenuOpen)
    {
        // Set the window pos to the mouse pos
        ImGui::SetNextWindowPos(ImVec2(io->MouseClickedPos[1][0] - 2, io->MouseClickedPos[1][1] - 2), ImGuiCond_Appearing);
        // Draw a window with no boarder or menu
        if(ImGui::Begin("Right Click Menu", &rightMenuOpen, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_AlwaysAutoResize))
        {
            // Draw the library credits
            glakCredits();
        }
        // End the window
        ImGui::End();
    }
}

///
/// draw()
/// Called every loop
///
void draw()
{
    // Draw our object
    obj.draw();
}

///
/// init()
/// This will only be run once (when the application starts)
///
void init()
{
    // Load a shader from file (doesn't require recompiling to change shader but less portable)
    // shader.init(glakReadFile("shaders/vshader.glsl"), glakReadFile("shaders/fshader.glsl"));

    // Create a shader (requires recompiling to change shader but more portable)
    shader.init(R"(
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
})",R"(
#version 330 core

in vec4 fColor;

out vec4 pColor;

void main() 
{ 
    pColor = fColor;
})");

    // Enable OpenGL Z buffer
    glEnable(GL_DEPTH_TEST);

    // Add a shader to the object
    obj.shader.resize(1);
    obj.shader[0] = shader;

    // Add a mesh to the object
    obj.mesh.resize(1);
    obj.mesh[0].material = 0;

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
        obj.mesh[0].vertex.resize(attrib.vertices.size()/3);
        for(size_t i = 0, j = 0; i < obj.mesh[0].vertex.size(); i++)
        {
            obj.mesh[0].vertex[i].pos.x = attrib.vertices[j++]/10.0f;
            obj.mesh[0].vertex[i].pos.y = attrib.vertices[j++]/10.0f;
            obj.mesh[0].vertex[i].pos.z = attrib.vertices[j++]/10.0f;
        }

        // Copy index information
        obj.mesh[0].index.resize(shape[0].mesh.indices.size());
        for(size_t i = 0; i < obj.mesh[0].index.size(); i++)
        {
            obj.mesh[0].index[i] = shape[0].mesh.indices[i].vertex_index;
        }
    }
    else
    {
        // If we couldn't find teapot.obj, create a flat colored plane
        cout << "Error opening OBJ file " << objdir << endl;

        // Add vertices
        obj.mesh[0].vertex.resize(4);
        obj.mesh[0].vertex[0].pos = {-0.7f, -0.7f, 0.0f, 1.0f};
        obj.mesh[0].vertex[1].pos = {-0.7f,  0.7f, 0.0f, 1.0f};
        obj.mesh[0].vertex[2].pos = { 0.7f,  0.7f, 0.0f, 1.0f};
        obj.mesh[0].vertex[3].pos = { 0.7f, -0.7f, 0.0f, 1.0f};

        // Add colors
        obj.mesh[0].vertex[0].col = {1.0f, 0.0f, 0.0f, 1.0f};
        obj.mesh[0].vertex[1].col = {0.0f, 1.0f, 0.0f, 1.0f};
        obj.mesh[0].vertex[2].col = {0.0f, 0.0f, 1.0f, 1.0f};
        obj.mesh[0].vertex[3].col = {1.0f, 1.0f, 1.0f, 1.0f};

        // Add indices for triagles ((0, 1, 2), (0, 2, 3))
        obj.mesh[0].index.resize(6);
        obj.mesh[0].index = {0, 1, 2, 0, 2, 3};
    }

    obj.updateBuffer();
}

///
/// destroy()
/// Called only once (at application shutdown)
///
void destroy()
{
}

///
/// main()
/// You shouldn't need to modify this code unless you need to change some SDL or ImGui settings
///
int main(int argc, char** argv)
{
    // Setup SDL
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
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
    window = SDL_CreateWindow("ImGui SDL2 + OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    // Initialise gl3w
    gl3wInit();

    // Initialise ImGui
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui_ImplSdlGL3_Init(window);
    ImGui::StyleColorsDark();
    style = &ImGui::GetStyle();
    style->WindowRounding = 0;

    // Run init function
    init();

    bool done = false;
    while(!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdlGL3_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
        }
        ImGui_ImplSdlGL3_NewFrame(window);

        // Run loop function
        loop();

        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glClearColor(clearCol[0], clearCol[1], clearCol[2], clearCol[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Run draw function
        draw();

        ImGui::Render();
        ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(window);
    }

    // Run destroy function
    destroy();
    
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}