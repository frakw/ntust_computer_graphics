#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_pos;

out V_OUT
{
    vec3 world_pos;
    vec3 world_normal;
    vec2 texture_pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 world_tagent;
} v_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_projection;
    mat4 u_view;
};

uniform mat4 u_model;
uniform vec2 u_texture_size;

uniform sampler2D u_texture_height;   //height map

void main()
{
    float height;
    vec3 normal;

    vec4 world_pos = vec4(position, 1.0);
    world_pos = u_model * world_pos;
    
    v_out.world_pos = world_pos.xyz;
    // v_out.world_normal = transpose(inverse(mat3(u_model))) * normal;
    v_out.world_normal = transpose(inverse(mat3(u_model))) * vec3(0.0, 1.0, 0.0);
    v_out.texture_pos = texture_pos;

    //useless 
    v_out.ambient = vec3(1.0, 1.0, 1.0);
    v_out.diffuse = vec3(1.0, 1.0, 1.0);
    v_out.specular = vec3(1.0, 1.0, 1.0);
    v_out.world_tagent = vec3(0.0, 0.0, 0.0);

    gl_Position = u_projection * u_view * u_model * world_pos;
}