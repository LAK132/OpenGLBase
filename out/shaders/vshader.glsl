#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vColor;

out vec4 fColor;

void main()
{
    gl_Position = vPosition;
    fColor = vColor;
}