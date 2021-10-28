#version 430 core
in V_OUT
{
   vec3 normal;
   vec3 pos;
   vec4 pos_light_space;
} f_in;

out vec4 f_color;


struct Material
{
    //sampler2D t_diffuse;
    //sampler2D t_specular;
	//sampler2D t_emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct DirectLight
{                   //offset
	vec3 direction; //0
    vec3 ambient;   //16
    vec3 diffuse;   //32
    vec3 specular;  //48
};

layout (std140, binding = 1) uniform Light
{
    DirectLight u_directLight;
};
layout (std140, binding = 2) uniform View
{
    vec3 u_view_dir;    //0
    vec3 u_view_pos;    //16
};



uniform Material u_material;

uniform sampler2D u_shadow_map;
uniform int u_have_shadow;

vec3 computeDirectLight(DirectLight light, vec3 normal, vec3 view_dir);
float ShadowCalculation(vec4 pos_light_space);
void main()
{
    vec3 normal = normalize(f_in.normal);
    vec3 color = computeDirectLight(u_directLight, normal, u_view_dir);
    f_color = vec4(color, 1.0f);
}
vec3 computeDirectLight(DirectLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(light.direction);

	//ambient
	vec3 ambient = light.ambient * u_material.ambient;	//texture() return vec4

	//diffuse
	float diff = max(dot(normal, -light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * u_material.diffuse;

	//specular
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
	vec3 specular = light.specular * spec * u_material.specular;
    vec3 lighting = ambient + diffuse + specular;
    if(u_have_shadow == 1)
    {
        float shadow = ShadowCalculation(f_in.pos_light_space);                      
        shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
        //shadow = 1.0f;
        lighting = (ambient + (1.0 - shadow) * (diffuse + specular));     
    }
   
	return lighting;
}
float ShadowCalculation(vec4 pos_light_space)
{
    // perform perspective divide
    vec3 proj_coords = pos_light_space.xyz / pos_light_space.w;
    // Transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closest_depth = texture(u_shadow_map, proj_coords.xy).r; 
    // Get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(f_in.normal);
    vec3 light_dir = normalize(u_view_pos - f_in.pos);
    float bias = max(0.001 * (1.0 - dot(normal, light_dir)), 0.0001);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(u_shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(u_shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf_depth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shsadow at 0.0 when outside the far_plane region of the light's frustum.
    if(proj_coords.z > 1.0)
        shadow = 0.0;
    
    return shadow;
}