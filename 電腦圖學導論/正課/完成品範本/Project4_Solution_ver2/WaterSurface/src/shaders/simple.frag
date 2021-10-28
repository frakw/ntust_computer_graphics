out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} f_in;

uniform vec3 u_color;

uniform sampler2D u_texture;

void main()
{   
    // vec4 info = texture2D(u_texture, f_in.texture_coordinate);
    // vec4 info = vec4(0.0, 1.0, 0.0, 0.0);

    // vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);
    
    f_color = vec4(texture2D(u_texture, f_in.texture_coordinate).rrr, 1.0);
    // f_color = vec4(normal, 1.0f);
    // f_color = vec4(0.0, 1.0, 0.0, 1.0);
}