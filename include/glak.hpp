/*
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
SOFTWARE.
*/

// C++ 
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <exception>
using std::exception;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <vector>
using std::vector;
#include <stddef.h>
#include <stdint.h>

// OpenGL
#include <GL/gl3w.h>
#ifndef GLAK_DISABLE_3D
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

#ifndef GLAK_H
#define GLAK_H

#ifndef cstring

extern bool debugOn;
extern bool throwOn;
#define DEBUG if(debugOn) std::cout << __FILE__ << "(" << std::dec << __LINE__ << ")" << std::endl
#define THROW(x) if(throwOn) throw x

#ifndef GLAK_DISABLE_3D
void glakReadFile(string src, string* dst);
string glakReadFile(string src);
void glakInitShader(GLuint program, const string& src, GLenum type);
void glakLinkProgram(GLuint program);

struct glakVertex
{
    glm::vec4 pos;
    glm::vec4 col;
    glm::vec3 norm;
    glm::vec2 coord;
};

#define GLAK_VERTEX_ATTRIB_CONSTS(X, E) static const GLintptr X ## Off = offsetof(glakVertex, glakVertex::X); static const size_t X ## Size = E;

struct glakVertexConst
{
    GLAK_VERTEX_ATTRIB_CONSTS(pos,      sizeof(glakVertex::pos) / sizeof(glakVertex::pos.x))        // posSize, posOff
    GLAK_VERTEX_ATTRIB_CONSTS(col,      sizeof(glakVertex::col) / sizeof(glakVertex::col.x))        // colSize, colOff
    GLAK_VERTEX_ATTRIB_CONSTS(norm,     sizeof(glakVertex::norm) / sizeof(glakVertex::norm.x))      // normSize, normOff
    GLAK_VERTEX_ATTRIB_CONSTS(coord,    sizeof(glakVertex::coord) / sizeof(glakVertex::coord.x))    // coordSize, coordOff
};

struct glakShader
{
    GLint prevProgram = NULL;
    shared_ptr<GLint> program;
    GLint position = -1;
    GLint color = -1;
    GLint normal = -1;
    GLint texCoord = -1;
    glakShader();
    glakShader(const glakShader& other);
    glakShader(glakShader&& other);
    glakShader(string vshader, string fshader);
    glakShader& operator=(const glakShader& other);
    glakShader& operator=(glakShader&& other);
    ~glakShader();
    void init(string vshader, string fshader);
    void enable();
    void disable();
    GLint operator*() const;
};

struct glakBuffer
{
    bool isInit = false;
    GLuint vertArr = 0, vertBuff = 0, indxBuff = 0;
    void init();
    ~glakBuffer();
};

typedef GLushort glakIndex;

struct glakMesh
{
private:
    size_t index_count = 0;
    size_t vertex_count = 0;
public:
    size_t material = 0;
    glakBuffer buffer;
    vector<glakVertex> vertex;
    vector<glakIndex> index;
    void updateBuffer();
    void draw();
};

struct glakTransform
{
    glm::mat4 translation = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 transform = glm::mat4(1.0f);
    glakTransform& addTranslation(const glm::vec3& displace);   
    glakTransform& addTranslation(glm::vec3&& displace);   
    glakTransform& setTranslation(const glm::vec3& position);
    glakTransform& setTranslation(glm::vec3&& position);
    glakTransform& addScale(const glm::vec3& sca);
    glakTransform& addScale(glm::vec3&& sca);
    glakTransform& setScale(const glm::vec3& sca);
    glakTransform& setScale(glm::vec3&& sca);
    glakTransform& clear();
    glm::mat4& make(bool clearmats = false);
    glm::mat4& append(bool clearmats = false);
    glm::mat4& prepend(bool clearmats = false);
    template<typename T>
    glakTransform& addRotation(T angle, const glm::vec3& axis)
    {
        rotation = glm::rotate(rotation, angle, axis);
        return *this;
    }
    template<typename T>
    glakTransform& addRotation(T angle, glm::vec3&& axis)
    {
        return addRotation(angle, axis);
    }
    template<typename T>
    glakTransform& setRotation(T angle, const glm::vec3& axis)
    {
        rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
        return *this;
    }
    template<typename T>
    glakTransform& setRotation(T angle, glm::vec3&& axis)
    {
        return setRotation(angle, axis);
    }
};

struct glakObject
{
    glakTransform transform;
    vector<shared_ptr<glakShader>> shader;
    vector<glakMesh> mesh;
    void updateBuffer();
    void draw();
};

#endif // GLAK_DISABLE_3D

#endif // GLAK_H

/*
#define GLAK_HANDLE_MAIN
#define GLAK_MULTITHREAD
//*/

#ifdef GLAK_HANDLE_MAIN

#define SDL_MAIN_HANDLED
#include <SDL.h>

#ifdef GLAK_MULTITHREAD

#include <thread>
using std::thread;
#include <atomic>
using std::atomic;
using std::atomic_bool;
#include <mutex>
using std::mutex;

struct glakLoopData
{
    atomic_bool run;
    mutex draw_mtx;
    mutex update_mtx;
    SDL_Window* window;
    SDL_GLContext glContext;
    void* userData;
};

#else

struct glakLoopData
{
    SDL_Window* window;
    SDL_GLContext glContext;
    void* userData;
};

#endif // GLAK_MULTITHREAD

int main();
void update_loop(glakLoopData* ld);
#ifdef GLAK_MULTITHREAD
void draw_loop(glakLoopData* ld);
#endif

void update(glakLoopData* ld, double deltaTime);
void draw(glakLoopData* ld, double deltaTime);

void init(glakLoopData* ld);
void destroy(glakLoopData* ld);

#endif // GLAK_HANDLE_MAIN

#endif // GLAK_H

/*
#define GLAK_IMPLEMENTATION
//*/

#ifdef GLAK_IMPLEMENTATION

bool debugOn = true;
bool throwOn = true;

#ifndef GLAK_DISABLE_3D

void glakReadFile(string src, string* dst)
{
    FILE* ptr = fopen(src.c_str(), "r");
    if(ptr == NULL) 
    {
        cout << "Failed to open file " << src.c_str() << endl;
        *dst = "";
        return;
    }
    fseek(ptr, 0L, SEEK_END);
    size_t size = ftell(ptr);
    fseek(ptr, 0L, SEEK_SET);
    dst->resize(size);
    fread((void*)&((*dst)[0]), 1, size, ptr);
    *dst += '\0';
    fclose(ptr);
}

string glakReadFile(string src)
{
    string rtn = "";
    glakReadFile(src, &rtn);
    return rtn;
}

void glakInitShader(GLuint program, const string& src, GLenum type)
{
    if(src == "")
    {
        cout << "Error reading shader " << src.c_str() << endl;
        throw exception();//"Error reading shader");
    }
    GLuint shader = glCreateShader(type);
    GLchar* shadercstr = (GLchar*)&(src[0]);
    glShaderSource(shader, 1, &shadercstr, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        cout << "Error compiling shader " << src.c_str() << endl;
        GLint msgSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgSize);
        string msg; msg.resize(msgSize);
        glGetShaderInfoLog(shader, msgSize, NULL, &(msg[0]));
        cout << msg.c_str() << endl;
        throw exception();//msg.c_str());
    }
    glAttachShader(program, shader);
}

void glakLinkProgram(GLuint program)
{
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        cout << "Shader program failed to link" << endl;
        GLint msgSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &msgSize);
        string msg; msg.resize(msgSize);
        glGetProgramInfoLog(program, msgSize, NULL, &(msg[0]));
        cout << msg.c_str() << endl;
        throw exception();//msg.c_str());
    }
}

// glakShader
glakShader::glakShader(){}

glakShader::glakShader(const glakShader& other)
{
    program = other.program;
    position = other.position;
    normal = other.normal;
    color = other.color;
    texCoord = other.texCoord;
}

glakShader::glakShader(glakShader&& other)
{
    program = other.program;
    position = other.position;
    normal = other.normal;
    color = other.color;
    texCoord = other.texCoord;
}

glakShader::glakShader(string vshader, string fshader)
{
    init(vshader, fshader);
}

glakShader& glakShader::operator=(const glakShader& other)
{
    program = other.program;
    position = other.position;
    normal = other.normal;
    color = other.color;
    texCoord = other.texCoord;
    return *this;
}

glakShader& glakShader::operator=(glakShader&& other)
{
    program = other.program;
    position = other.position;
    normal = other.normal;
    color = other.color;
    texCoord = other.texCoord;
    return *this;
}

glakShader::~glakShader()
{
    if (program.unique())
    {
        glDeleteProgram(*program);
    }
}

void glakShader::init(string vshader, string fshader)
{
    program = make_shared<GLint>(glCreateProgram());
    glakInitShader(*program, vshader, GL_VERTEX_SHADER);
    glakInitShader(*program, fshader, GL_FRAGMENT_SHADER);
    glakLinkProgram(*program);

    position =  glGetAttribLocation(*program, "vPosition");
    normal =    glGetAttribLocation(*program, "vNormal");
    color =     glGetAttribLocation(*program, "vColor");
    texCoord =  glGetAttribLocation(*program, "vTexCoord");
}

#define GLAK_ENABLE_ATTRIB(N, S, T, NO, PS, O) if(N >= 0) {glEnableVertexAttribArray(N); glVertexAttribPointer(N, S, T, NO, PS, (GLvoid*)O);}

void glakShader::enable()
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
    if (program.use_count() <= 0) return;
    glUseProgram(*program);
    GLAK_ENABLE_ATTRIB(position,    glakVertexConst::posSize,   GL_FLOAT, GL_FALSE, sizeof(glakVertex), glakVertexConst::posOff)
    GLAK_ENABLE_ATTRIB(color,       glakVertexConst::colSize,   GL_FLOAT, GL_FALSE, sizeof(glakVertex), glakVertexConst::colOff)
    GLAK_ENABLE_ATTRIB(normal,      glakVertexConst::normSize,  GL_FLOAT, GL_FALSE, sizeof(glakVertex), glakVertexConst::normOff)
    GLAK_ENABLE_ATTRIB(texCoord,    glakVertexConst::coordSize, GL_FLOAT, GL_FALSE, sizeof(glakVertex), glakVertexConst::coordOff)
}

void glakShader::disable()
{
    if (position >= 0)   glDisableVertexAttribArray(position);
    if (normal >= 0)     glDisableVertexAttribArray(normal);
    if (color >= 0)      glDisableVertexAttribArray(color);
    if (texCoord >= 0)   glDisableVertexAttribArray(texCoord);
    glUseProgram(prevProgram);
}

GLint glakShader::operator*() const
{
    return *program;
}

// glakBuffer
void glakBuffer::init()
{
    if(isInit)
    {
        glDeleteBuffers(1, &indxBuff);
        glDeleteBuffers(1, &vertBuff);
        glDeleteVertexArrays(1, &vertArr);
    }
    glGenVertexArrays(1, &vertArr);
    glGenBuffers(1, &vertBuff);
    glGenBuffers(1, &indxBuff);
    isInit = true;
}

glakBuffer::~glakBuffer()
{
    if(!isInit) return;
    glDeleteBuffers(1, &indxBuff);
    glDeleteBuffers(1, &vertBuff);
    glDeleteVertexArrays(1, &vertArr);
    isInit = false;
}

// glakMesh
void glakMesh::updateBuffer()
{
    buffer.init();
    
    glBindVertexArray(buffer.vertArr);

    vertex_count = vertex.size();
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertBuff);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glakVertex), &(vertex[0]), GL_STATIC_DRAW);

    index_count = index.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.indxBuff);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(glakIndex), &(index[0]), GL_STATIC_DRAW);
}

void glakMesh::draw()
{
    glBindVertexArray(buffer.vertArr);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}

// glakTransform
glakTransform& glakTransform::addTranslation(const glm::vec3& displace)
{
    translation = glm::translate(translation, displace);
    return *this;
}

glakTransform& glakTransform::addTranslation(glm::vec3&& displace)
{
    return addTranslation(displace);
}

glakTransform& glakTransform::setTranslation(const glm::vec3& position)
{
    translation = glm::translate(glm::mat4(1.0f), position);
    return *this;
}

glakTransform& glakTransform::setTranslation(glm::vec3&& position)
{
    return setTranslation(position);
}

glakTransform& glakTransform::addScale(const glm::vec3& sca)
{
    scale = glm::scale(scale, sca + glm::vec3(1.0f, 1.0f, 1.0f));
    return *this;
}

glakTransform& glakTransform::addScale(glm::vec3&& sca)
{
    return addScale(sca);
}

glakTransform& glakTransform::setScale(const glm::vec3& sca)
{
    scale = glm::scale(glm::mat4(1.0f), sca);
    return *this;
}

glakTransform& glakTransform::setScale(glm::vec3&& sca)
{
    return setScale(sca);
}

glakTransform& glakTransform::clear()
{
    translation = glm::mat4(1.0f);
    rotation = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    return *this;
}

glm::mat4& glakTransform::make(bool clearmats)
{
    transform = rotation * scale * transform;
    if(clearmats) clear();
    return transform;
}

glm::mat4& glakTransform::append(bool clearmats)
{
    transform = translation * rotation * scale * transform;
    if(clearmats) clear();
    return transform;
}

glm::mat4& glakTransform::prepend(bool clearmats)
{
    transform = transform * translation * rotation * scale;
    if(clearmats) clear();
    return transform;
}

// glakObject
void glakObject::updateBuffer()
{
    for(auto it = mesh.begin(); it != mesh.end(); it++)
    {
        it->updateBuffer();
    }
}

void glakObject::draw()
{
    shared_ptr<glakShader> prev = nullptr;
    for(auto it = mesh.begin(); it != mesh.end(); it++)
    {
        if (it->material < shader.size() && &(shader[it->material]) != nullptr)
        {
            if (prev != shader[it->material])
            {
                if(prev != nullptr) prev->disable();
                prev = shader[it->material];
                prev->enable();
            }
            it->draw();
        }
    }
    if(prev != nullptr) prev->disable();
}

#endif // GLAK_DISABLE_3D

#ifdef GLAK_HANDLE_MAIN

int main()
{
    glakLoopData ld;
    init(&ld);

    ld.run = true;

    update(&ld, 0.0f); // Make sure to update once before drawing
    
    #ifdef GLAK_MULTITHREAD
    thread draw(draw_loop, &ld);
    #endif

    update_loop(&ld);

    #ifdef GLAK_MULTITHREAD
    draw.join();
    #endif

    SDL_GL_MakeCurrent(ld.window, ld.glContext);
    destroy(&ld);

    return 0;
}

#ifdef GLAK_MULTITHREAD

void update_loop(glakLoopData* ld)
{
    uint64_t LAST = 0;
    uint64_t NOW = SDL_GetPerformanceCounter();
    double deltaTime = 0;

    while(ld->run)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = ((NOW - LAST) * 1000) / (double) SDL_GetPerformanceFrequency();

        ld->update_mtx.lock();
        ld->draw_mtx.lock();
        ld->update_mtx.unlock();

        SDL_GL_MakeCurrent(ld->window, ld->glContext);
        update(ld, deltaTime);
        SDL_GL_MakeCurrent(0, 0);

        ld->draw_mtx.unlock();
    }
}

void draw_loop(glakLoopData* ld)
{
    uint64_t LAST = SDL_GetPerformanceCounter();
    uint64_t NOW = SDL_GetPerformanceCounter();
    double deltaTime = 0;
    double targetDrawTime = 12.0;

    while(ld->run)
    {
        for(int i = 0; i < 2 && ld->run; i++)
        {
            ld->update_mtx.lock();
            ld->draw_mtx.lock();
            ld->update_mtx.unlock();

            SDL_GL_MakeCurrent(ld->window, ld->glContext);
            draw(ld, deltaTime);
            SDL_GL_MakeCurrent(0, 0);

            ld->draw_mtx.unlock();
        }
        do 
        {
            LAST = NOW;
            NOW = SDL_GetPerformanceCounter();
            deltaTime += ((NOW - LAST) * 1000) / (double) SDL_GetPerformanceFrequency();
        } while (deltaTime < targetDrawTime); // sleep the draw thread
    }
}

#else

void update_loop(glakLoopData* ld)
{
    uint64_t LAST = 0;
    uint64_t NOW = SDL_GetPerformanceCounter();
    double deltaTime = 0;

    // SDL_GL_MakeCurrent(ld->window, ld->glContext);
    while(ld->run)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = ((NOW - LAST) * 1000) / (double) SDL_GetPerformanceFrequency();

        update(ld, deltaTime);
        draw(ld, deltaTime);
    }
    // SDL_GL_MakeCurrent(0, 0);
}

#endif // GLAK_MULTITHREAD

#endif // GLAK_HANDLE_MAIN

#endif // GLAK_IMPLEMENTATION