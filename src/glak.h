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
#include <GL/gl3w.h>
#include <SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <tiny_obj_loader.h>

// Implement DQ transforms http://simonstechblog.blogspot.com.au/2011/11/dual-quaternion.html
//http://www.chinedufn.com/dual-quaternion-shader-explained/

#define DEBUG cout << __FILE__ << "(" << __LINE__ << ")" << endl;

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

string glakReadFile(const string& src);
void glakReadFile(const string& src, string& dst);
void glakInitShader(GLuint program, const string& filedir, GLenum type);
void glakLinkProgram(GLuint program);

struct glakVertex
{
    glakVec4 pos;
    glakVec4 col;
    glakVec3 norm;
    glakVec2 coord;
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

#define glakIndex GLushort

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
    void draw();//glakShader* shader);
};

struct glakObject
{
    vector<glakShader> shader;
    vector<glakMesh> mesh;
    void updateBuffer();
    void draw();
};

void glakCredits();

#endif