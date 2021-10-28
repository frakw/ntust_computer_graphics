in vec2 coord;

layout (location = 0) out vec4 fragColor;

#if defined(WATER_INTERACTIVE)
uniform sampler2D u_water;
uniform vec2 u_delta;

void main() {
    /* get vertex info */
    vec4 info = texture2D(u_water, coord);
    
    /* calculate average neighbor height */
    vec2 dx = vec2(u_delta.x, 0.0);
    vec2 dy = vec2(0.0, u_delta.y);
    float average = (
    texture2D(u_water, coord - dx).r + //texture2D(u_material.texture_displacement, coord - dx).r * 0.1 +
    texture2D(u_water, coord - dy).r + //texture2D(u_material.texture_displacement, coord - dy).r * 0.1+
    texture2D(u_water, coord + dx).r + //texture2D(u_material.texture_displacement, coord + dx).r * 0.1+ 
    texture2D(u_water, coord + dy).r //+ texture2D(u_material.texture_displacement, coord + dy).r * 0.1
    ) * 0.25;
    
    /* change the velocity to move toward the average */
    info.g += (average - info.r) * 2.0;
    // info.g += (average - (info.r + height.r * 0.1)) * 2.0;
    
    /* attenuate the velocity a little so waves do not last forever */
    info.g *= 0.995;
    
    /* move the vertex along the velocity */
    info.r += info.g;
    // info.r = height.r * 0.1;

    fragColor = info;
    // fragColor = vec4(1.0, 0.0, 0.0, 0.0);
}

#elif defined(WATER_HEIGHTMAP)
uniform float u_scale;

void main()
{
    float height = texture2D(u_material.texture_displacement, coord).r;

    fragColor = vec4(height * u_scale, 0.0, 0.0, 0.0);
}

#elif defined(WATER_SINE)
uniform vec2 u_direction;
uniform float u_wavelength;
uniform float u_time;
uniform float u_amplitude;

const float PI = 3.1415926;


void main()
{
    float k = 2 * PI / u_wavelength;
    float c = sqrt(9.8 / k);
    vec2 d = normalize(u_direction);
    float f = k * (dot(d, coord) - c * u_time);

    // p.x += d.x * (a * cos(f));
    // p.y = a * sin(f);
    // p.z += d.y * (a * cos(f));
    fragColor = vec4(u_amplitude * sin(f), 0.0, 0.0, 0.0);
}

#endif