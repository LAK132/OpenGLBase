#include "main.h"
#define LAK_MAIN_IMPLEM
#include <lak/main.hpp>

///
/// event()
/// Always called from the main thread, use this thread to access SDL events
///
void event(lak::loopData* ld)
{
    userData_t& ud = ld->userData;
    SDL_Event event;
    while (SDL_PollEvent(&event) && ld->events.size() < 100)
    {
        ld->events.push_back(event);
    }
    ImGui_ImplSdlGL3_NewFrame(ld->window);
}

///
/// loop()
/// Called every loop
///
void update(lak::loopData* ld)
{
    userData_t& ud = ld->userData;
    for(auto it = ld->events.begin(); it != ld->events.end(); it++)
    {
        switch(it->type)
        {
            case SDL_QUIT: {
                ld->running = false;
            } break;
            case SDL_WINDOWEVENT: {
                if(it->window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    ud.screenw = it->window.data1;
                    ud.screenh = it->window.data2;
                    glViewport(0, 0, ud.screenw, ud.screenh);
                    ud.scene.camera->projection = glm::perspective(90.0f * glm::pi<float>() / 180.0f, (float)ud.screenw / (float)ud.screenh, 0.01f, 100.0f);
                }
            } break;
            case SDL_KEYDOWN: {
                switch(it->key.keysym.scancode)
                {
                    case SDL_SCANCODE_LEFT: {
                        ud.scene.cameraBoom->transform.rotVel.y = 1;
                        DEBUG("Left\n");
                    } break;
                    case SDL_SCANCODE_UP: {
                        ud.scene.cameraBoom->transform.rotVel.x = 1;
                        DEBUG("Up\n");
                    } break;
                    case SDL_SCANCODE_RIGHT: {
                        ud.scene.cameraBoom->transform.rotVel.y = -1;
                        DEBUG("Right\n");
                    } break;
                    case SDL_SCANCODE_DOWN: {
                        ud.scene.cameraBoom->transform.rotVel.x = -1;
                        DEBUG("Down\n");
                    } break;
                }
            } break;
            case SDL_KEYUP: {
                switch(it->key.keysym.scancode)
                {
                    case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_RIGHT:
                        ud.scene.cameraBoom->transform.rotVel.y = 0;
                        break;
                    case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_DOWN:
                        ud.scene.cameraBoom->transform.rotVel.x = 0;
                        break;
                }
            } break;
        }
        ImGui_ImplSdlGL3_ProcessEvent(&*it);
    }
    ld->events.clear();
    
    ud.scene.cameraBoom->transform.update(ld->updateDelta);
    ud.scene.camera->transform.make();

    ImGui::NewFrame();

    // static bool is not reset every time loop() is called, effectively global variable but cannot be accessed outside of the loop() scope
    static bool rightMenuOpen = true;
    // Check if ImGui wants to use the mouse
    if(!ud.io->WantCaptureMouse)
    {
        // If ImGui is not using the mouse, check if the right button has been clicked
        if(ud.io->MouseClicked[1])
        {
            cout << "right mouse clicked\n";
            // Toggle rightMenuOpen
            rightMenuOpen = !rightMenuOpen;
        }
    }
    // Check if ImGui wants to use the keyboard
    if(!ud.io->WantCaptureKeyboard)
    {

    }
    
    // If rightMenuOpen draw a small menu
    if(rightMenuOpen)
    {
        // Set the window pos to the mouse pos
        ImGui::SetNextWindowPos(ImVec2(ud.io->MouseClickedPos[1][0] - 2, ud.io->MouseClickedPos[1][1] - 2), ImGuiCond_Appearing);
        // Draw a window with no boarder or menu
        if(ImGui::Begin("Right Click Menu", &rightMenuOpen, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Draw Time %d", (size_t)std::ceil(1.0f / ld->drawDelta));
            ImGui::Text("Update Time %f", ld->updateDelta * 1000.0);
            // Draw the library credits
            credits();
        }
        // End the window
        ImGui::End();
    }

    {
        if(ImGui::Begin("Splitter Window"), NULL, ImGuiWindowFlags_NoSavedSettings)
        {
            static ImGuiStyle& style = ImGui::GetStyle();
            ImVec2 contentSize = ImGui::GetWindowContentRegionMax();
            contentSize.x = ImGui::GetWindowContentRegionWidth();
            static float size1 = contentSize.x / 2;
            static float size2 = contentSize.x / 2;
            ImGui::Splitter(style.WindowPadding.x, &size1, &size2, 8.0f, 8.0f, contentSize.x);

            ImGui::BeginChild("Left", ImVec2(size1, -1), true, ImGuiWindowFlags_NoSavedSettings);
            {
                ImGui::Text("Window Max x %f", size1);
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild("Right", ImVec2(size2, -1), false, ImGuiWindowFlags_NoSavedSettings);
            {
                ImVec2 contentSize2 = ImGui::GetWindowContentRegionMax();
                contentSize2.x = ImGui::GetWindowContentRegionWidth();
                static float size3 = contentSize2.x / 2;
                static float size4 = contentSize2.x / 2;
                ImGui::Splitter(style.WindowPadding.y, &size3, &size4, 8.0f, 8.0f, contentSize2.y, false);
                ImGui::BeginChild("Top", ImVec2(-1, size3), true, ImGuiWindowFlags_NoSavedSettings);
                {
                    ImGui::Text("Window Max x %f", size3);
                }
                ImGui::EndChild();
                ImGui::Spacing();
                ImGui::BeginChild("Bottom", ImVec2(-1, size4), true, ImGuiWindowFlags_NoSavedSettings);
                {
                    ImGui::Text("Window Max x %f", size4);
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();

        }
        ImGui::End();
    }

    ImGui::Render();
}


///
/// draw()
/// Called every loop
///
void draw(lak::loopData* ld)
{
    userData_t& ud = ld->userData;
    glViewport(0, 0, (int)ud.io->DisplaySize.x, (int)ud.io->DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw our object
    ud.shader->setUniform("projection", &(ud.scene.camera->projection[0][0]));
    ud.shader->setUniform("view", &ud.scene.camera->getTransform());
    ud.scene.obj->draw();

    if(ImDrawData* draw_data = ImGui::GetDrawData()) 
        ImGui_ImplSdlGL3_RenderDrawData(draw_data);
}

///
/// init()
/// This will only be run once (when the application starts)
///
void init(lak::loopData* ld)
{
    userData_t& ud = ld->userData;
    // Setup SDL
    assert(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0);

    ud.screenw = 1280;
    ud.screenh = 720;

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
    ld->window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ud.screenw, ud.screenh, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    ld->context = SDL_GL_CreateContext(ld->window);
    SDL_GL_MakeCurrent(ld->window, ld->context);
    // /*
    if (SDL_GL_SetSwapInterval(-1) == -1) // adaptive vsync
    {
        SDL_GL_SetSwapInterval(1); // standard vsync
    }
    // */
    // SDL_GL_SetSwapInterval(0); // no vsync

    // Initialise gl3w
    gl3wInit();

    // Initialise ImGui
    ImGui::CreateContext();
    ud.io = &ImGui::GetIO();
    ImGui_ImplSdlGL3_Init(ld->window);
    ImGui::StyleColorsDark();
    ud.style = &ImGui::GetStyle();
    ud.style->WindowRounding = 0;
    ImGui_ImplSdlGL3_NewFrame(ld->window);

    // Loadind a shader from file doesn't require recompiling to change shader but less portable
    // Creating a shader requires recompiling to change shader but more portable
    // So try loading but if it fails use the compiled shader
    string vshader = readFile("vshader.glsl");
    string fshader = readFile("fshader.glsl");
    if(vshader == "")
    {
        vshader = R"(
#version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 fColor;
out vec3 fNormal;
out vec2 fTexCoord;
out vec3 fPosition;
out vec3 fEye;

const vec4 WUP = vec4(0.0, 0.0, 0.0, 1.0);

void main()
{
    mat4 projview = projection * view;
    mat4 invprojview = transpose(inverse(projview));
    vec4 vertpos = model * vPosition; // object -> world space

    fTexCoord = vTexCoord;
    fColor = vColor;
    fEye = vec3(WUP * invprojview); // screen -> camera -> world space
    fNormal = mat3(model) * vNormal; // object -> world space (no translation/scale)
    fPosition = vertpos.xyz;

    gl_Position = projview * vertpos; // world -> camera -> screen space
})";
    }
    if(fshader == "")
    {
        fshader = R"(
#version 330 core

smooth in vec4 fColor;
smooth in vec3 fEye;
smooth in vec3 fNormal;
smooth in vec3 fPosition;
smooth in vec2 fTexCoord;

uniform sampler2D albedo;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

#define MAX_LIGHTS 6
struct light {
    vec3 position;
    vec4 color; // w is intensity
};
uniform int lightCount;
uniform light lights[MAX_LIGHTS];

out vec4 pColor;

void main() 
{
    vec3 normal = normalize(fNormal);
    vec3 viewDir = normalize(fEye - fPosition);
    vec4 baseColor = fColor * texture(albedo, fTexCoord);

    pColor = ambient;

    for(int i = 0; i < lightCount && i < MAX_LIGHTS; i++)
    {
        vec3 lightDir = normalize(lights[i].position - fPosition);
        float dNL = max(dot(normal, lightDir), 0.0f);
        vec4 color = diffuse;
        color = diffuse * baseColor;
        vec4 lambert = color * lights[i].color * dNL;

        vec3 halfVec = normalize(lightDir + viewDir);
        float dNH = max(dot(normal, halfVec), 0.0f);
        vec4 phong = specular * lights[i].color * pow(dNH, shininess);
        pColor += lambert + phong;
    }
})";
    }
    
    /*
     * model:       model transform     model -> world      updated on model change
     * view:        camera transform    world -> camera     updated on camera change
     * projection:  screen transform    camera -> screen    updated on screen change
     * screenPosition = projection * view * model * vertPosition
    */

    ud.shader = make_shared<lak::shader_t>();
    ud.shader->init(vshader, fshader);

    ud.scene.camera->parent = ud.scene.cameraBoom;
    ud.scene.camera->transform.clear();
    ud.scene.camera->transform.setTranslation({0.0f, 0.0f, -3.0f});
    ud.shader->setUniform("view", &ud.scene.camera->getTransform());

    ud.scene.camera->projection = glm::perspective(90.0f * glm::pi<float>() / 180.0f, (float)ud.screenw / (float)ud.screenh, 0.01f, 100.0f);
    ud.shader->setUniform("projection", &ud.scene.camera->projection[0][0]);

    int lightCount = 3;
    ud.shader->setUniform("lightCount", &lightCount);
    ud.shader->setUniform("lights[0].position", &glm::vec3(1.5f, 1.5f, 1.5f));
    ud.shader->setUniform("lights[0].color", &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ud.shader->setUniform("lights[1].position", &glm::vec3(-1.5f, 1.5f, 1.5f));
    ud.shader->setUniform("lights[1].color", &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ud.shader->setUniform("lights[2].position", &glm::vec3(1.5f, -1.5f, -1.5f));
    ud.shader->setUniform("lights[2].color", &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    ud.shader->setUniform("ambient", &glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ud.shader->setUniform("diffuse", &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ud.shader->setUniform("specular", &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    float shine = 100.0f;
    ud.shader->setUniform("shininess", &shine);

    ud.shader->setUniform("model", &glm::mat4(1.0f));

    // Enable OpenGL Z buffer
    glEnable(GL_DEPTH_TEST);

    // ifstream imgstrm("wood.ppm", std::ios::binary|ifstream::in);
    // ud.image = make_shared<lak::imageRGBA8_t>(lak::readPNM<lak::imageRGBA8_t>(imgstrm));
    ud.image = make_shared<lak::imageRGBA8_t>(4, vector<lak::colorRGBA8_t>{
        {0, 0, 0, 255}, {255, 255, 255, 255}, {0, 0, 0, 255}, {255, 255, 255, 255}, 
        {255, 255, 255, 255}, {0, 0, 0, 255}, {255, 255, 255, 255}, {0, 0, 0, 255}, 
        {0, 0, 0, 255}, {255, 255, 255, 255}, {0, 0, 0, 255}, {255, 255, 255, 255}, 
        {255, 255, 255, 255}, {0, 0, 0, 255}, {255, 255, 255, 255}, {0, 0, 0, 255}
    });
    ud.texture = make_shared<lak::texture_t>();
    ud.texture->generate(GL_TEXTURE_2D, 0, GL_RGBA, 0, *ud.image, {
        lak::texparam_t(GL_TEXTURE_MIN_FILTER, GL_NEAREST), 
        lak::texparam_t(GL_TEXTURE_MAG_FILTER, GL_NEAREST),
        lak::texparam_t(GL_TEXTURE_WRAP_S, GL_REPEAT),
        lak::texparam_t(GL_TEXTURE_WRAP_T, GL_REPEAT)
    });

    // Add a mesh to the object
    ud.scene.obj->mesh.resize(1);
    ud.scene.obj->mesh[0].shader = ud.shader;
    ud.scene.obj->mesh[0].textures["albedo"] = ud.texture;

    vector<uint8_t> indices = {
        0, 1, 2, 
        0, 2, 3,

        0, 4, 5, 
        0, 5, 1,

        1, 5, 6, 
        1, 6, 2,

        2, 6, 7, 
        2, 7, 3,

        3, 7, 4, 
        3, 4, 0,

        4, 5, 6, 
        4, 6, 7
    };

    vector<glm::vec4> vertPos = {
        { 0.7f, -0.7f,  0.7f, 1.0f},
        { 0.7f,  0.7f,  0.7f, 1.0f},
        {-0.7f,  0.7f,  0.7f, 1.0f},
        {-0.7f, -0.7f,  0.7f, 1.0f},
        { 0.7f, -0.7f, -0.7f, 1.0f},
        { 0.7f,  0.7f, -0.7f, 1.0f},
        {-0.7f,  0.7f, -0.7f, 1.0f},
        {-0.7f, -0.7f, -0.7f, 1.0f}
    };

    // Add vertices
    lak::mesh_t::element_t& vpos = ud.scene.obj->mesh[0].elements["vPosition"];
    glm::vec4* vposp = vpos.init<glm::vec4>(indices.size());
    for(size_t i = 0; i < indices.size(); i++)
    {
        vposp[i] = vertPos[indices[i]];
    }
    vpos.normalized = false;
    vpos.active = true;

    // Add colors
    lak::mesh_t::element_t& vcol = ud.scene.obj->mesh[0].elements["vColor"];
    glm::vec4* vcolp = vcol.init<glm::vec4>(indices.size());
    vcolp[0]  = {1.0f, 0.0f, 0.0f, 1.0f}; vcolp[1]  = {1.0f, 0.0f, 0.0f, 1.0f}; vcolp[2]  = {1.0f, 0.0f, 0.0f, 1.0f};
    vcolp[3]  = {1.0f, 0.0f, 0.0f, 1.0f}; vcolp[4]  = {1.0f, 0.0f, 0.0f, 1.0f}; vcolp[5]  = {1.0f, 0.0f, 0.0f, 1.0f};

    vcolp[6]  = {0.0f, 1.0f, 0.0f, 1.0f}; vcolp[7]  = {0.0f, 1.0f, 0.0f, 1.0f}; vcolp[8]  = {0.0f, 1.0f, 0.0f, 1.0f};
    vcolp[9]  = {0.0f, 1.0f, 0.0f, 1.0f}; vcolp[10] = {0.0f, 1.0f, 0.0f, 1.0f}; vcolp[11] = {0.0f, 1.0f, 0.0f, 1.0f};

    vcolp[12] = {0.0f, 0.0f, 1.0f, 1.0f}; vcolp[13] = {0.0f, 0.0f, 1.0f, 1.0f}; vcolp[14] = {0.0f, 0.0f, 1.0f, 1.0f};
    vcolp[15] = {0.0f, 0.0f, 1.0f, 1.0f}; vcolp[16] = {0.0f, 0.0f, 1.0f, 1.0f}; vcolp[17] = {0.0f, 0.0f, 1.0f, 1.0f};

    vcolp[18] = {0.0f, 1.0f, 1.0f, 1.0f}; vcolp[19] = {0.0f, 1.0f, 1.0f, 1.0f}; vcolp[20] = {0.0f, 1.0f, 1.0f, 1.0f};
    vcolp[21] = {0.0f, 1.0f, 1.0f, 1.0f}; vcolp[22] = {0.0f, 1.0f, 1.0f, 1.0f}; vcolp[23] = {0.0f, 1.0f, 1.0f, 1.0f};

    vcolp[24] = {1.0f, 0.0f, 1.0f, 1.0f}; vcolp[25] = {1.0f, 0.0f, 1.0f, 1.0f}; vcolp[26] = {1.0f, 0.0f, 1.0f, 1.0f};
    vcolp[27] = {1.0f, 0.0f, 1.0f, 1.0f}; vcolp[28] = {1.0f, 0.0f, 1.0f, 1.0f}; vcolp[29] = {1.0f, 0.0f, 1.0f, 1.0f};

    vcolp[30] = {1.0f, 1.0f, 0.0f, 1.0f}; vcolp[31] = {1.0f, 1.0f, 0.0f, 1.0f}; vcolp[32] = {1.0f, 1.0f, 0.0f, 1.0f};
    vcolp[33] = {1.0f, 1.0f, 0.0f, 1.0f}; vcolp[34] = {1.0f, 1.0f, 0.0f, 1.0f}; vcolp[35] = {1.0f, 1.0f, 0.0f, 1.0f};
    vcol.normalized = false;
    vcol.active = true;

    lak::mesh_t::element_t& vtex = ud.scene.obj->mesh[0].elements["vTexCoord"];
    glm::vec2* vtexp = vtex.init<glm::vec2>(indices.size());
    vtexp[0]  = {0.0f, 0.0f}; vtexp[1]  = {1.0f, 0.0f}; vtexp[2]  = {1.0f, 1.0f};
    vtexp[3]  = {0.0f, 0.0f}; vtexp[4]  = {1.0f, 1.0f}; vtexp[5]  = {0.0f, 1.0f};
    
    vtexp[6]  = {0.0f, 0.0f}; vtexp[7]  = {1.0f, 0.0f}; vtexp[8]  = {1.0f, 1.0f};
    vtexp[9]  = {0.0f, 0.0f}; vtexp[10] = {1.0f, 1.0f}; vtexp[11] = {0.0f, 1.0f};
    
    vtexp[12] = {0.0f, 0.0f}; vtexp[13] = {1.0f, 0.0f}; vtexp[14] = {1.0f, 1.0f};
    vtexp[15] = {0.0f, 0.0f}; vtexp[16] = {1.0f, 1.0f}; vtexp[17] = {0.0f, 1.0f};
    
    vtexp[18] = {0.0f, 0.0f}; vtexp[19] = {1.0f, 0.0f}; vtexp[20] = {1.0f, 1.0f};
    vtexp[21] = {0.0f, 0.0f}; vtexp[22] = {1.0f, 1.0f}; vtexp[23] = {0.0f, 1.0f};
    
    vtexp[24] = {0.0f, 0.0f}; vtexp[25] = {1.0f, 0.0f}; vtexp[26] = {1.0f, 1.0f};
    vtexp[27] = {0.0f, 0.0f}; vtexp[28] = {1.0f, 1.0f}; vtexp[29] = {0.0f, 1.0f};
    
    vtexp[30] = {0.0f, 0.0f}; vtexp[31] = {1.0f, 0.0f}; vtexp[32] = {1.0f, 1.0f};
    vtexp[33] = {0.0f, 0.0f}; vtexp[34] = {1.0f, 1.0f}; vtexp[35] = {0.0f, 1.0f};
    vtex.normalized = false;
    vtex.active = true;

    lak::mesh_t::element_t& norm = ud.scene.obj->mesh[0].elements["vNormal"];
    glm::vec3* normp = norm.init<glm::vec3>(indices.size());
    for(int i = 0; i < indices.size(); i++) normp[i] = glm::normalize((glm::vec3)vposp[i]);
    norm.normalized = true;
    norm.active = true;

    ud.scene.obj->mesh[0].index.clear();

    ud.scene.obj->update();

    glViewport(0, 0, 500, 500);
    glClearColor(ud.clearCol[0], ud.clearCol[1], ud.clearCol[2], ud.clearCol[3]);

    event(ld);
    update(ld);
    draw(ld);
}

///
/// destroy()
/// Called only once (at application shutdown)
///
void destroy(lak::loopData* ld)
{
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(ld->context);
    SDL_DestroyWindow(ld->window);

    SDL_Quit();
}