const float IOR_AIR = 1.0;
const float IOR_WATER = 1.333;
const vec3 abovewaterColor = vec3(0.25, 1.0, 1.25);
const vec3 underwaterColor = vec3(0.4, 0.9, 1.0);
const float poolHeight = 1.0;
const vec3 light = vec3(-1.0, 2.0, -2.0);
// uniform vec3 light;
// uniform vec3 sphereCenter;
// uniform float sphereRadius;
uniform sampler2D u_tiles;
uniform sampler2D u_caustic;
uniform sampler2D u_water;
  
vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax) {
    vec3 tMin = (cubeMin - origin) / ray;
    vec3 tMax = (cubeMax - origin) / ray;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    return vec2(tNear, tFar);
}
  
// float intersectSphere(vec3 origin, vec3 ray, vec3 sphereCenter, float sphereRadius) {
//     vec3 toSphere = origin - sphereCenter;
//     float a = dot(ray, ray);
//     float b = 2.0 * dot(toSphere, ray);
//     float c = dot(toSphere, toSphere) - sphereRadius * sphereRadius;
//     float discriminant = b*b - 4.0*a*c;
//     if (discriminant > 0.0) {
//     float t = (-b - sqrt(discriminant)) / (2.0 * a);
//     if (t > 0.0) return t;
//     }
//     return 1.0e6;
// }

// vec3 getSphereColor(vec3 point) {
//     vec3 color = vec3(0.5);

//     /* ambient occlusion with walls */
//     color *= 1.0 - 0.9 / pow((1.0 + sphereRadius - abs(point.x)) / sphereRadius, 3.0);
//     color *= 1.0 - 0.9 / pow((1.0 + sphereRadius - abs(point.z)) / sphereRadius, 3.0);
//     color *= 1.0 - 0.9 / pow((point.y + 1.0 + sphereRadius) / sphereRadius, 3.0);

//     /* caustics */
//     vec3 sphereNormal = (point - sphereCenter) / sphereRadius;
//     vec3 refractedLight = refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
//     float diffuse = max(0.0, dot(-refractedLight, sphereNormal)) * 0.5;
//     vec4 info = texture2D(u_water, point.xz * 0.5 + 0.5);
//     if (point.y < info.r) {
//     vec4 caustic = texture2D(u_caustic, 0.75 * (point.xz - point.y * refractedLight.xz / refractedLight.y) * 0.5 + 0.5);
//     diffuse *= caustic.r * 4.0;
//     }
//     color += diffuse;

//     return color;
// }

vec3 getWallColor(vec3 point) {
    float scale = 0.5;

    vec3 wallColor;
    vec3 normal;
    if (abs(point.x) > 0.999) {
    wallColor = texture2D(u_tiles, point.yz * 0.5 + vec2(1.0, 0.5)).rgb;
    normal = vec3(-point.x, 0.0, 0.0);
    } else if (abs(point.z) > 0.999) {
    wallColor = texture2D(u_tiles, point.yx * 0.5 + vec2(1.0, 0.5)).rgb;
    normal = vec3(0.0, 0.0, -point.z);
    } else {
    wallColor = texture2D(u_tiles, point.xz * 0.5 + 0.5).rgb;
    normal = vec3(0.0, 1.0, 0.0);
    }

    scale /= length(point); /* pool ambient occlusion */
    // scale *= 1.0 - 0.9 / pow(length(point - sphereCenter) / sphereRadius, 4.0); /* sphere ambient occlusion */

    /* caustics */
    vec3 refractedLight = -refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    float diffuse = max(0.0, dot(refractedLight, normal));
    vec4 info = texture2D(u_water, point.xz * 0.5 + 0.5);
    if (point.y < info.r) {
    vec4 caustic = texture2D(u_caustic, 0.75 * (point.xz - point.y * refractedLight.xz / refractedLight.y) * 0.5 + 0.5);
    scale += diffuse * caustic.r * 2.0 * caustic.g;
    } else {
    /* shadow for the rim of the pool */
    vec2 t = intersectCube(point, refractedLight, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
    diffuse *= 1.0 / (1.0 + exp(-200.0 / (1.0 + 10.0 * (t.y - t.x)) * (point.y + refractedLight.y * t.y - 2.0 / 12.0)));

    scale += diffuse * 0.5;
    }

    return wallColor * scale;
}