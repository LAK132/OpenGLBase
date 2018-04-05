#include "glak.h"

void glakReadShaderFile(const string& src, string& dst)
{
    FILE* ptr = fopen(src.c_str(), "r");
    if(ptr == NULL) 
    {
        dst = "";
        return;
    }
    fseek(ptr, 0L, SEEK_END);
    size_t size = ftell(ptr);
    fseek(ptr, 0L, SEEK_SET);
    dst.resize(size);
    fread((void*)&(dst[0]), 1, size, ptr);
    dst += '\0';
    fclose(ptr);
}

string glakReadShaderFile(const string& src)
{
    string rtn = "";
    glakReadShaderFile(src, rtn);
    return rtn;
}

void glakInitShader(GLuint program, const string& shaderstr, GLenum type)
{
    if(shaderstr == "")
    {
        cout << "Error reading shader " << shaderstr.c_str() << endl;
        throw exception("Error reading shader");
    }
    GLuint shader = glCreateShader(type);
    GLchar* shadercstr = (GLchar*)&(shaderstr[0]);
    glShaderSource(shader, 1, &shadercstr, NULL);
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        cout << "Error compiling shader " << shaderstr.c_str() << endl;
        GLint msgSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgSize);
        string msg; msg.resize(msgSize);
        glGetShaderInfoLog(shader, msgSize, NULL, &(msg[0]));
        cout << msg.c_str() << endl;
        throw exception(msg.c_str());
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
        throw exception(msg.c_str());
    }
}

glakShader::glakShader(){}

glakShader::glakShader(const glakShader& other)
{
    program = other.program;
}

glakShader::~glakShader()
{
    if (program.unique())
        glDeleteProgram(*program);
}

void glakShader::init(const string& vshader, const string& fshader)
{
    program = make_shared<GLuint>(glCreateProgram());
    glakInitShader(*program, vshader, GL_VERTEX_SHADER);
    glakInitShader(*program, fshader, GL_FRAGMENT_SHADER);
    glLinkProgram(*program);
}

void glakShader::use()
{
    if (program.use_count() > 0)
        glUseProgram(*program);
}

void glakObject::init()
{
    glGenVertexArrays(1, &vtxArr);
    glGenBuffers(1, &vtxBuff);
    glGenBuffers(1, &colBuff);
}

glakObject::~glakObject()
{
    glDeleteBuffers(1, &colBuff);
    glDeleteBuffers(1, &vtxBuff);
    glDeleteVertexArrays(1, &vtxArr);
}

void glakCredits()
{
    ImGui::PushID("Credits");
    if(ImGui::TreeNode("ImGui"))
    {
        ImGui::Text(R"(
https://github.com/ocornut/imgui

The MIT License (MIT)

Copyright (c) 2014-2018 Omar Cornut

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
        )");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("glm"))
    {
        ImGui::Text(R"(
https://github.com/g-truc/glm

The Happy Bunny License (Modified MIT License)

Copyright (c) 2005 - 2016 G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions
of the Software.

Restrictions: By making use of the Software for military purposes, you choose to make a Bunny unhappy.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
        )");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("gl3w"))
    {
        ImGui::Text("https://github.com/skaslev/gl3w");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("GLFW3"))
    {
        ImGui::Text("http://www.glfw.org/");
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Miniz"))
    {
        ImGui::Text("https://github.com/richgel999/miniz");
        ImGui::TreePop();
    }
    ImGui::PopID();
}