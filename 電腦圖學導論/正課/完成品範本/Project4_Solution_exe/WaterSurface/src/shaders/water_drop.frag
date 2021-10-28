in vec2 coord;

layout (location = 0) out vec4 fragColor;

const float PI = 3.141592653589793;
uniform sampler2D u_water;
uniform vec2 u_center;
uniform float u_radius;
uniform float u_strength;

void main() {
    /* get vertex info */
    vec4 info = texture2D(u_water, coord);
    
    /* add the drop to the height */
    float drop = max(0.0, 1.0 - length(u_center * 0.5 + 0.5 - coord) / u_radius);
    drop = 0.5 - cos(drop * PI) * 0.5;
    info.r += drop * u_strength;
    
    fragColor = info;
    // fragColor = vec4(0.0, 1.0, 0.0, 0.0);
}