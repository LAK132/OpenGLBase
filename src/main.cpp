#include "main.h"

///
/// loop()
/// Called every loop
///
void update(atomic_bool* run, SDL_Window** window, double deltaTime, void** userDataPtr)
{
    userData_t*& ud = *(userData_t**)userDataPtr;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&event);
        if (event.type == SDL_QUIT) *run = false;
    }
    ImGui_ImplSdlGL3_NewFrame(*window);

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
            glakCredits();
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
void draw(atomic_bool* run, SDL_Window** window, void** userDataPtr)
{
    userData_t*& ud = *(userData_t**)userDataPtr;
    glViewport(0, 0, (int)ud->io->DisplaySize.x, (int)ud->io->DisplaySize.y);
    glClearColor(ud->clearCol[0], ud->clearCol[1], ud->clearCol[2], ud->clearCol[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our object
    ud->obj.draw();

    ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(*window);
}

///
/// init()
/// This will only be run once (when the application starts)
///
int init(SDL_Window** window, SDL_GLContext* glContext, void** userDataPtr)
{
    *userDataPtr = (void*)new userData_t();
    userData_t*& ud = *(userData_t**)userDataPtr;
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
    *window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    *glContext = SDL_GL_CreateContext(*window);
    if (SDL_GL_SetSwapInterval(-1) == -1) // adaptive vsync
    {
        SDL_GL_SetSwapInterval(1); // standard vsync
    }

    // Initialise gl3w
    gl3wInit();

    // Initialise ImGui
    ImGui::CreateContext();
    ud->io = &ImGui::GetIO();
    ImGui_ImplSdlGL3_Init(*window);
    ImGui::StyleColorsDark();
    ud->style = &ImGui::GetStyle();
    ud->style->WindowRounding = 0;

    // Load a shader from file (doesn't require recompiling to change shader but less portable)
    // shader.init(glakReadFile("shaders/vshader.glsl"), glakReadFile("shaders/fshader.glsl"));

    // Create a shader (requires recompiling to change shader but more portable)
    ud->shader.init(
R"(
#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

out vec4 fColor;

void main()
{
    gl_Position.xy = vPosition.xy;
    gl_Position.z = vPosition.z - 0.5;
    fColor = vColor;
})",
R"(
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
    ud->obj.shader.resize(1);
    ud->obj.shader[0] = ud->shader;

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
        ud->obj.mesh[0].vertex.resize(attrib.vertices.size()/3);
        for(size_t i = 0, j = 0; i < ud->obj.mesh[0].vertex.size(); i++)
        {
            ud->obj.mesh[0].vertex[i].pos.x = attrib.vertices[j++]/10.0f;
            ud->obj.mesh[0].vertex[i].pos.y = attrib.vertices[j++]/10.0f;
            ud->obj.mesh[0].vertex[i].pos.z = attrib.vertices[j++]/10.0f;
        }

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
        cout << "Error opening OBJ file " << objdir << endl;

        // Add vertices
        ud->obj.mesh[0].vertex.resize(4);
        ud->obj.mesh[0].vertex[0].pos = {-0.7f, -0.7f, 0.0f, 1.0f};
        ud->obj.mesh[0].vertex[1].pos = {-0.7f,  0.7f, 0.0f, 1.0f};
        ud->obj.mesh[0].vertex[2].pos = { 0.7f,  0.7f, 0.0f, 1.0f};
        ud->obj.mesh[0].vertex[3].pos = { 0.7f, -0.7f, 0.0f, 1.0f};

        // Add colors
        ud->obj.mesh[0].vertex[0].col = {1.0f, 0.0f, 0.0f, 1.0f};
        ud->obj.mesh[0].vertex[1].col = {0.0f, 1.0f, 0.0f, 1.0f};
        ud->obj.mesh[0].vertex[2].col = {0.0f, 0.0f, 1.0f, 1.0f};
        ud->obj.mesh[0].vertex[3].col = {1.0f, 1.0f, 1.0f, 1.0f};

        // Add indices for triagles ((0, 1, 2), (0, 2, 3))
        ud->obj.mesh[0].index.resize(6);
        ud->obj.mesh[0].index = {0, 1, 2, 0, 2, 3};
    }

    ud->obj.updateBuffer();

    return 0;
}

///
/// destroy()
/// Called only once (at application shutdown)
///
int destroy(SDL_Window** window, SDL_GLContext* glContext, void** userDataPtr)
{
    userData_t*& ud = *(userData_t**)userDataPtr;
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(*glContext);
    SDL_DestroyWindow(*window);

    delete ud;

    SDL_Quit();

    return 0;
}