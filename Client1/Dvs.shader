#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 Color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection *  view * model * vec4(aPos, 1.0);
	Color = gl_Position;//位置作为颜色
	//Color = vec4(0, 0, gl_Position.z, 1.0);//高度值作为颜色
	//gl_Position = vec4(aPos, 1.0);
}