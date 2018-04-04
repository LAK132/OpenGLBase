#include "main.h"

GLFWwindow* window = nullptr;
ImGuiIO* io = nullptr;
ImVec4 clearCol = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
vector<GLuint> shaders;

void loop()
{
    // ImGui_ImplGlfwGL3_NewFrame();
}

void draw()
{
    // ImGui::Render();
}

void init()
{
    shaders.push_back(glCreateProgram());
    GLAK::initShader(shaders.back(), "shaders\\vshader.glsl", GL_VERTEX_SHADER);
    GLAK::initShader(shaders.back(), "shaders\\fshader.glsl", GL_FRAGMENT_SHADER);
    GLAK::linkProgram(shaders.back());
    glUseProgram(shaders.back());
    glEnable(GL_DEPTH_TEST);

    // ImGui
    // ImGui::CreateContext();
    // io = &ImGui::GetIO();
    // ImGui_ImplGlfwGL3_Init(window, true);
    // ImGui::StyleColorsDark();
}

void destroy()
{
    // ImGui_ImplGlfwGL3_Shutdown();
    // ImGui::DestroyContext();
    // glfwTerminate();
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

    shaders.clear();

    init();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        loop();

        int dispW, dispH;
        glfwGetFramebufferSize(window, &dispW, &dispH);
        glViewport(0, 0, dispW, dispH);
        glClearColor(clearCol.x, clearCol.y, clearCol.z, clearCol.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw();
        
        glfwSwapBuffers(window);
    }

    destroy();

    return 0;
}