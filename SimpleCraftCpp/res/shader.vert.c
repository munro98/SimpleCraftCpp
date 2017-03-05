#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    //gl_Position = projection * view * model * vec4(position, 1.0f);
    //vec4 thing = vec4(float(blockPosition[0]), float(blockPosition[1]), float(blockPosition[2]), 0.0f);
    //vec4 thing = vec4(blockPosition, 0.0f);
    //gl_Position = projection * view * model * (vec4(position, 1.0f));
    gl_Position = projection * (vec4(position, 1.0f));
    FragPos = vec3(model * vec4(position, 0.0f));
    //Normal = normal;
    Normal = mat3(transpose(model)) * normal;
    //Normal = mat3(transpose(inverse(model))) * normal;//inversing is costly
    TexCoords = texCoords;
} 