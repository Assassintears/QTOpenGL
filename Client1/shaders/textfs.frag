#version 450 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;

void main()
{    
    // color = texture(text, TexCoords);
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}