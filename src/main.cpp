#include "main.h"

GLFWwindow* window = nullptr;
float clearCol[4] = {0.45f, 0.55f, 0.60f, 1.00f};
glakShader shader;
glakBuffer vtxObj;
glakObject obj;
ImGuiIO* io = nullptr;

void loop()
{
    ImGui_ImplGlfwGL3_NewFrame();

    static bool mainOpen = true;
    ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiCond_FirstUseEver);
    if(ImGui::Begin("OpenGL Demo", &mainOpen, ImGuiWindowFlags_MenuBar))
    {
        glakCredits();
        ImGui::End();
    }
}

void draw()
{
    obj.draw();

    ImGui::Render();
}

void init()
{
    vtxObj.init();

    shader.init(
        glakReadShaderFile("shaders\\vshader.glsl"), 
        glakReadShaderFile("shaders\\fshader.glsl"));

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

    // ImGui
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
}

void destroy()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

int main(int argc, char** argv)
{
    if(!glfwInit()) return 1;
    // glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    window = glfwCreateWindow(1280, 720, APP_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //vsync

    // gl3w
    gl3wInit();

    init();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        loop();

        int dispW, dispH;
        glfwGetFramebufferSize(window, &dispW, &dispH);
        glViewport(0, 0, dispW, dispH);
        glClearColor(clearCol[0], clearCol[1], clearCol[2], clearCol[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();
        
        glfwSwapBuffers(window);
    }

    destroy();

    return 0;
}