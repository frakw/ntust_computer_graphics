#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffAlbedo;
layout (location = 3) out vec4 gSpecAlbedo;

in E_OUT
{
    vec3 world_pos;
    vec3 world_normal;
    vec2 texture_pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    mat3 TBN;
} f_in;

// in V_OUT
// {
//     vec3 world_pos;
//     vec3 world_normal;
//     vec2 texture_pos;
//     vec3 world_tagent;
// } f_in;

struct Material
{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
    sampler2D texture_displacement;
    samplerCube texture_cubemap;
    float shininess;
};
layout (std140, binding = 2) uniform View
{
    vec3 u_view_dir;    //0
    vec3 u_view_pos;    //16
};

uniform Material u_material;

uniform bool u_use_normal_map;

void main()
{    
    vec3 normal = texture(u_material.texture_normal, f_in.texture_pos).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(f_in.TBN * normal);

    if(u_use_normal_map)
        normal = normalize(normal);
    else
        normal = normalize(f_in.world_normal);

    gNormal = normalize(normal);

    vec3 I = f_in.world_pos - u_view_pos;
    // I.y *= -1.0f; //?
    vec3 R = reflect(I, normal);
    
    vec3 reflect_color = vec3(texture(u_material.texture_cubemap, R));

    // Store the fragment position vector in the first gbuffer texture
    gPosition = f_in.world_pos;
    // Also store the per-fragment normals into the gbuffer

    gDiffAlbedo = vec4(reflect_color, 1.0);
    gSpecAlbedo = vec4(reflect_color, 1.0);
}