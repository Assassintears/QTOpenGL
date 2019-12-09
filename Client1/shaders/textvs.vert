#version 450 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 textcoord;

layout (std140, column_major) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(vertex, 1.0);
    // gl_Position = vec4(view[0][0], 0.0f, 0.0f, 1.0);
    TexCoords = textcoord;
}
