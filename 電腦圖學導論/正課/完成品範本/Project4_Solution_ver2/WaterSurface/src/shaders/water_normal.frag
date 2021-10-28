in vec2 coord;

layout (location = 0) out vec4 fragColor;

uniform sampler2D u_water;
uniform vec2 u_delta;

void main() {
    /* get vertex info */
    vec4 info = texture2D(u_water, coord);
    
    float dx = u_delta.x;
    float dz = u_delta.y;

    /* update the normal */
    float dy = texture2D(u_water, vec2(coord.x + dx, coord.y)).r - info.r;
    vec3 du = vec3(dx, dy, 0.0);
    
    dy = texture2D(u_water, vec2(coord.x, coord.y + dz)).r - info.r;
    vec3 dv = vec3(0.0, dy, dz);

    //normal
    info.ba = normalize(cross(dv, du)).xz;
    
    fragColor = info;
    // fragColor = vec4(0.0, 0.0, 0.0, 0.0);
}