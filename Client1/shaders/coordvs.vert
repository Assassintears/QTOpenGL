#version 450 core
layout (location = 0) in vec3 xPos;
layout (std140, column_major) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec4 Color;


void main()
{

    gl_Position = projection *  view * model * vec4(xPos, 1.0);
    Color = vec4(1.0, 1.0, 1.0, 1.0);
}
