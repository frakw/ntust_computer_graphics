layout(triangles, equal_spacing, ccw) in;

in C_OUT
{
   vec3 position;
} e_in[];

out E_OUT
{
   vec3 position;
   vec3 world_position;
} e_out;

// uniform sampler2D u_water;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);

void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    e_out.position = interpolate3D(e_in[0].position, e_in[1].position, e_in[2].position);
    vec4 info = texture2D(u_water, e_out.position.xz * 0.5 + 0.5);
    e_out.position.y += info.r;
    e_out.world_position = vec3(u_model * vec4(e_out.position, 1.0));
    gl_Position = u_projection * u_view * vec4(e_out.world_position, 1.0);
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 