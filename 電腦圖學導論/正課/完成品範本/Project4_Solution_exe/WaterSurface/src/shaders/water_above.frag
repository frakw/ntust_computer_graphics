out vec4 fragColor;

// in V_OUT
// {
//    vec3 position;
//    vec3 world_position;
// } f_in;


in E_OUT
{
   vec3 position;
   vec3 world_position;
} f_in;


// uniform sampler2D u_water;

vec3 getSurfaceRayColor(vec3 origin, vec3 ray, vec3 waterColor) {
    vec3 color;
    if (ray.y < 0.0) {
        vec2 t = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
        color = getWallColor(origin + ray * t.y);
    } else {
        vec2 t = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
        vec3 hit = origin + ray * t.y;
        if (hit.y < 2.0 / 12.0) {
        color = getWallColor(hit);
        } else {
        color = texture(u_material.texture_cubemap, ray).rgb;
        color += vec3(pow(max(0.0, dot(light, ray)), 5000.0)) * vec3(10.0, 8.0, 6.0);
        }
    }
    if (ray.y < 0.0) color *= waterColor;
    return color;
}

void main() {
    vec2 coord = f_in.position.xz * 0.5 + 0.5;
    vec4 info = texture2D(u_water, coord);

    /* make water look more "peaked" */
    for (int i = 0; i < 5; i++) {
        coord += info.ba * 0.005;
        info = texture2D(u_water, coord);
    }

    vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);
    vec3 incomingRay = f_in.world_position - u_camera_position;
    incomingRay = normalize(incomingRay);

    vec3 reflectedRay = reflect(incomingRay, normal);
    vec3 refractedRay = refract(incomingRay, normal, IOR_AIR / IOR_WATER);
    float fresnel = mix(0.25, 1.0, pow(1.0 - dot(normal, -incomingRay), 3.0));
    
    vec3 reflectedColor = getSurfaceRayColor(f_in.position, reflectedRay, abovewaterColor);
    vec3 refractedColor = getSurfaceRayColor(f_in.position, refractedRay, abovewaterColor);
    // vec3 reflectedColor = texture(u_material.texture_cubemap, reflectedRay).rgb * abovewaterColor;
    // vec3 refractedColor = texture(u_material.texture_cubemap, refractedRay).rgb * abovewaterColor;
    
    fragColor = vec4(mix(refractedColor, reflectedColor, fresnel), 1.0);
    // fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}