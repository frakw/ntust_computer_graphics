#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 coord;


out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offset;
uniform vec4 color;
uniform float scale = 10.0f;

void main()
{
    TexCoords = coord;
    ParticleColor = color;
    gl_Position = projection * view * model * vec4((pos * scale) + offset, 1.0);
}