#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, TexCoords);
}