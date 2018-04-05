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
#include <imgui.h>
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>

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

#ifndef GLAK_H
#define GLAK_H

string glakReadShaderFile(const string& src);
void glakReadShaderFile(const string& src, string& dst);
void glakInitShader(GLuint program, const string& filedir, GLenum type);
void glakLinkProgram(GLuint program);

struct glakShader
{
    shared_ptr<GLuint> program;
    glakShader();
    glakShader(const glakShader& other);
    ~glakShader();
    void init(const string& vshader, const string& fshader);
    void use();
    GLuint operator*() const;
};

struct glakObject
{
    GLuint vtxArr, vtxBuff, colBuff;
    ~glakObject();
    void init();
};

void glakCredits();

#endif