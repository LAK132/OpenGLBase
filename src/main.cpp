#include "main.h"

SDL_Window* window = nullptr;
SDL_GLContext glContext;
float clearCol[4] = {0.45f, 0.55f, 0.60f, 1.00f};
glakShader shader;
glakBuffer vtxObj;
glakObject obj;
ImGuiIO* io = nullptr;

void loop()
{
    static bool rightMenuOpen = true;
    if(!io->WantCaptureMouse)
    {
        if(io->MouseClicked[1])
        {
            cout << "Mouse 1 clicked" << endl;
            rightMenuOpen = !rightMenuOpen;
        }
    }
    if(!io->WantCaptureKeyboard)
    {

    }

    if(rightMenuOpen)
    {
        ImGui::SetNextWindowPos(io->MouseClickedPos[1], ImGuiCond_Appearing);
        //ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiCond_FirstUseEver);
        if(ImGui::Begin("Right Click Menu", &rightMenuOpen, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_AlwaysAutoResize))
        {
            glakCredits();
        }
        ImGui::End();
    }
}

void draw()
{
    obj.draw();
}

void init()
{
    vtxObj.init();

    shader.init(
        glakReadShaderFile("shaders/vshader.glsl"), 
        glakReadShaderFile("shaders/fshader.glsl"));

    glEnable(GL_DEPTH_TEST);

    obj.buff.init();

    obj.shader.resize(1);
    obj.shader[0] = shader;

    obj.vertex.resize(4);
    obj.vertex[0].pos = {-0.7f, -0.7f, 0.0f, 1.0f};
    obj.vertex[1].pos = {-0.7f,  0.7f, 0.0f, 1.0f};
    obj.vertex[2].pos = { 0.7f,  0.7f, 0.0f, 1.0f};
    obj.vertex[3].pos = { 0.7f, -0.7f, 0.0f, 1.0f};

    obj.vertex[0].col = {1.0f, 0.0f, 0.0f, 1.0f};
    obj.vertex[1].col = {0.0f, 1.0f, 0.0f, 1.0f};
    obj.vertex[2].col = {0.0f, 0.0f, 1.0f, 1.0f};
    obj.vertex[3].col = {1.0f, 1.0f, 1.0f, 1.0f};

    obj.polygon.resize(2);
    obj.polygon[0] = {{0, 1, 2}, 0};
    obj.polygon[1] = {{0, 2, 3}, 0};
}

void destroy()
{
}

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

    try{init();}catch(exception e){cout << "Error in init\n";}

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
        
        try{loop();}catch(exception e){cout << "Error in loop\n";}

        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glClearColor(clearCol[0], clearCol[1], clearCol[2], clearCol[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        try{draw();}catch(exception e){cout << "Error in draw\n";}

        ImGui::Render();
        ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(window);
    }

    try{destroy();}catch(exception e){cout << "Error in destroy\n";}
    
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}