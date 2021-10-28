out vec4 f_color;
  
in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D u_color_texture;

#if defined(DEFAULT)
void main()
{
    f_color = texture( u_color_texture, f_in.uv).rgba;
}
#elif defined(OFFSET)
uniform float u_time;
uniform vec2 u_resolution;
void main()
{
    f_color = texture( u_color_texture, 
        f_in.uv + 0.005*vec2( sin(u_time+u_resolution.x*f_in.uv.x),cos(u_time+u_resolution.y*f_in.uv.y)) ).rgba;
}
#elif defined(PIXELATION)
uniform vec2 u_resolution;

float vx_offset = 0.5f;
float pixel_w = 15.0f;
float pixel_h = 10.0f;
void main()
{
    vec3 tc = vec3(1.0, 0.0, 0.0);
    if (f_in.uv.x < (vx_offset-0.005))
    {
        float dx = pixel_w*(1./u_resolution.x);
        float dy = pixel_h*(1./u_resolution.y);
        vec2 coord = vec2(dx*floor(f_in.uv.x/dx),
                        dy*floor(f_in.uv.y/dy));
        tc = texture2D(u_color_texture, coord).rgb;
    }
    else if (f_in.uv.x>=(vx_offset+0.005))
    {
        tc = texture2D(u_color_texture, f_in.uv).rgb;
    }
    f_color = vec4(tc, 1.0f);
}
#endif