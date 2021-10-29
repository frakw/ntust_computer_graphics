#version 330 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D tex;

void main()
{
    color = (texture(tex, TexCoords) * ParticleColor);
    //color = vec4(0.5,0.5,0.5,1.0f);
}  