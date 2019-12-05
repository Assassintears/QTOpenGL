#version 330 core
out vec4 FragColor;
in vec4 Color;


void main()
{
	vec3 p = normalize(vec3(Color.x, Color.y, Color.z));
	
	FragColor = vec4(p, 1.0);
}