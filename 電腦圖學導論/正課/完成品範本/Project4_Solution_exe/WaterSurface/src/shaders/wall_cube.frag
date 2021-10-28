in V_OUT
{
   vec3 position;
} f_in;

out vec4 fragColor;

// uniform sampler2D u_water;

void main() 
{
    fragColor = vec4(getWallColor(f_in.position), 1.0);
    // fragColor = vec4(0.7, 0.7, 0.7, 1.0);
    vec4 info = texture2D(u_water, f_in.position.xz * 0.5 + 0.5);
    if (f_in.position.y < info.r) {
        fragColor.rgb *= underwaterColor * 1.2;
    }
    // fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}