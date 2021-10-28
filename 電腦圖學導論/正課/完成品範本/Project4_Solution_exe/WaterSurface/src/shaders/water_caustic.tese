layout(triangles, equal_spacing, ccw) in;

in C_OUT
{
   vec3 position;
//    vec3 world_position;
} e_in[];

out E_OUT
{
    vec3 old_position;
    vec3 new_position;
    vec3 ray;
} e_out;


/* project the ray onto the plane */
vec3 project(vec3 origin, vec3 ray, vec3 refracted_light) {
    vec2 tcube = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
    origin += ray * tcube.y;
    float tplane = (-1.0 - origin.y) / refracted_light.y;
    return origin + refracted_light * tplane;

    // float tplane = (1.0 - origin.y) / refracted_light.y;
    // return origin + refracted_light * tplane;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 

void main() {
    vec3 position = interpolate3D(e_in[0].position, e_in[1].position, e_in[2].position);

    vec4 info = texture2D(u_water, position.xz * 0.5 + 0.5);
    info.ba *= 0.5;
    vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);
    
    /* project the vertices along the refracted vertex ray */
    vec3 refracted_light = refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    e_out.ray = refract(-light, normal, IOR_AIR / IOR_WATER);
    e_out.old_position = project(position.xyz, refracted_light, refracted_light);
    e_out.new_position = project(position.xyz + vec3(0.0, info.r, 0.0), e_out.ray, refracted_light);
    
    gl_Position = vec4(0.75 * (e_out.new_position.xz + refracted_light.xz / refracted_light.y), 0.0, 1.0);
}

