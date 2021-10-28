in vec2 coord;

layout (location = 0) out vec4 fragColor;

uniform sampler2D u_water;
uniform vec2 u_delta;

void main() {
    /* get vertex info */
    vec4 info = texture2D(u_water, coord);
    
    /* update the normal */
    vec3 dx = vec3(
        u_delta.x,
        texture2D(u_water, vec2(coord.x + u_delta.x, coord.y)).r - info.r,
        0.0);
    vec3 dy = vec3(
        0.0,
        texture2D(u_water, vec2(coord.x, coord.y + u_delta.y)).r - info.r,
        u_delta.y);
    info.ba = normalize(cross(dy, dx)).xz;
    
    fragColor = info;
    // fragColor = vec4(0.0, 0.0, 0.0, 0.0);
}