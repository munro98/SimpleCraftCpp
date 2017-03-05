#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 viewprojection;

out vec3 Normal;
out vec2 TexCoords;

void main()
{
    gl_Position = viewprojection * (vec4(position, 1.0f));
    Normal = normal;
    TexCoords = texCoords;
} 