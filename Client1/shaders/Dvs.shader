#version 450 core
layout (location = 0) in vec3 aPos;
layout (std140, column_major) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
out vec4 Color;
uniform mat4 model;

void main()
{
        gl_Position = projection *  view * model * vec4(aPos, 1.0);
        Color = vec4(0.5f, 0.7f, 0.2f, 1.0f);
}
