#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (std140, column_major) uniform Matrices
{
    mat4 projection;
    mat4 model;
};
uniform mat4 camera;

out vec3 camPos;
out vec3 camNormal;

void main()
{
    gl_Position = projection * camera * model * vec4(aPos, 1.0);
    camPos = vec3(camera * model * vec4(aPos, 1.0));
    camNormal = mat3(transpose(inverse(camera * model))) * aNormal;
}
