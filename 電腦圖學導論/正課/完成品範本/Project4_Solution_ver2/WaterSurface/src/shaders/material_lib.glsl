struct Material
{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
    sampler2D texture_displacement;
    samplerCube texture_cubemap;
    float shininess;
};
uniform Material u_material;
