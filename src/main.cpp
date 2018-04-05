#include "main.h"

GLFWwindow* window = nullptr;
float clearCol[4] = {0.45f, 0.55f, 0.60f, 1.00f};
glakShader shader;
glakObject vtxObj;
ImGuiIO* io = nullptr;

GLuint vao, vbuffer, cbuffer;

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
    ImGui::Render();
}

void init()
{
    shader.init(
        glakReadShaderFile("shaders\\vshader.glsl"), 
        glakReadShaderFile("shaders\\fshader.glsl"));

    glEnable(GL_DEPTH_TEST);

    vtxObj.init();

    glBindBuffer(GL_ARRAY_BUFFER, vtxObj.vtxBuff);
    // glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(points4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vtxObj.colBuff);
    // glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(points4), NULL, GL_STATIC_DRAW);

    // GLuint vPosition = glGetAttribLocation(shader, "vPosition");
    // glEnableVertexAttribArray(vPosition);
    // glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

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