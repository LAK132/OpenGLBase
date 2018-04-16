#include <cmath>
#include <iostream>
#include <string.h>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <exception>
using std::exception;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <vector>
using std::vector;
#include <stddef.h>
#include <imgui.h>
#include <GL\gl3w.h>
#include <SDL.h>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_EPSILON
#define M_EPSILON 1.0e-07
#endif
#ifndef M_DEGTORAD
#define M_DEGTORAD(x) ((x*M_PI)/180.0)
#endif
#ifndef M_RADTODEG
#define M_RADTODEG(x) ((x*180.0)/M_PI)
#endif
#ifndef glakVec4
#define glakVec4 glm::vec4
#endif
#ifndef glakVec3
#define glakVec3 glm::vec3
#endif
#ifndef glakVec2
#define glakVec2 glm::vec2
#endif

#ifndef GLAK_H
#define GLAK_H

string glakReadShaderFile(const string& src);
void glakReadShaderFile(const string& src, string& dst);
void glakInitShader(GLuint program, const string& filedir, GLenum type);
void glakLinkProgram(GLuint program);

struct glakVertex
{
    glakVec4 pos;
    glakVec4 col;
    glakVec3 norm;
    glakVec2 coord;
};

struct glakVertexConst
{
    static const size_t posSize = sizeof(glakVertex::pos) / sizeof(glakVertex::pos.x);
    static const size_t colSize = sizeof(glakVertex::col) / sizeof(glakVertex::col.x);
    static const size_t normSize = sizeof(glakVertex::norm) / sizeof(glakVertex::norm.x);
    static const size_t coordSize = sizeof(glakVertex::coord) / sizeof(glakVertex::coord.x);
    static const GLintptr posOff = offsetof(glakVertex, glakVertex::pos);
    static const GLintptr colOff = offsetof(glakVertex, glakVertex::col);
    static const GLintptr normOff = offsetof(glakVertex, glakVertex::norm);
    static const GLintptr coordOff = offsetof(glakVertex, glakVertex::coord);
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
    ~glakShader();
    void init(const string& vshader, const string& fshader);
    void enable();
    void disable();
    GLint operator*() const;
};

struct glakBuffer
{
    GLuint vtxArr = 0, vtxBuff = 0, idxBuff = 0;
    ~glakBuffer();
    void init();
};

struct glakPolygon
{
    GLushort index[3];
    uint8_t material;
};

struct glakObject
{
    glakBuffer buff;
    vector<glakShader> shader;
    vector<glakVertex> vertex;
    vector<glakPolygon> polygon;
    void draw();
};

void glakCredits();

#endif