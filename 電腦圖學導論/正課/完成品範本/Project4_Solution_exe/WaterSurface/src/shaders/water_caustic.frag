out vec4 fragColor;

in E_OUT
{
    vec3 old_position;
    vec3 new_position;
    vec3 ray;
} f_in;

void main() {
   
    /* if the triangle gets smaller, it gets brighter, and vice versa */
    float old_area = length(dFdx(f_in.old_position)) * length(dFdy(f_in.old_position));
    float new_area = length(dFdx(f_in.new_position)) * length(dFdy(f_in.new_position));
    fragColor = vec4(old_area / new_area * 0.2, 1.0, 0.0, 0.0);
    // gl_FragColor = vec4(0.2, 0.2, 0.0, 0.0);
    
    vec3 refractedLight = refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    
    // /* compute a blob shadow and make sure we only draw a shadow if the player is blocking the light */
    // vec3 dir = (sphereCenter - f_in.new_position) / sphereRadius;
    // vec3 area = cross(dir, refractedLight);
    // float shadow = dot(area, area);
    // float dist = dot(dir, -refractedLight);
    // shadow = 1.0 + (shadow - 1.0) / (0.05 + dist * 0.025);
    // shadow = clamp(1.0 / (1.0 + exp(-shadow)), 0.0, 1.0);
    // shadow = mix(1.0, shadow, clamp(dist * 2.0, 0.0, 1.0));
    // fragColor.g = shadow;
    
    /* shadow for the rim of the pool */
    vec2 t = intersectCube(f_in.new_position, -refractedLight, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
    fragColor.r *= 1.0 / (1.0 + exp(-200.0 / (1.0 + 10.0 * (t.y - t.x)) * (f_in.new_position.y - refractedLight.y * t.y - 2.0 / 12.0)));
}