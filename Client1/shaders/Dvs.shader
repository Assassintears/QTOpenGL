#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (std140, column_major) uniform Matrices
{
    mat4 projection;
    mat4 model;
};
uniform mat4 camera;

out vec3 pos_in_cam;
out vec3 normal_in_cam;
out vec4 Color;

void main()
{
    gl_Position = projection * camera * model * vec4(aPos, 1.0);
    pos_in_cam = vec3(camera * model * vec4(aPos, 1.0));
    normal_in_cam = mat3(transpose(inverse(camera * model))) * aNormal;
    // Color = vec4(normal_in_cam, 1.0f);
}
