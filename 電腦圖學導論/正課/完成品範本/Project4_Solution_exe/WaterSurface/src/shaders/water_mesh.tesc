// define the number of CPs in the output patch
layout (vertices = 3) out;
// attributes of the input CPs
in V_OUT
{
   vec3 position;
} c_in[];

out C_OUT
{
   vec3 position;
} c_out[];

float getTessLevel(float distance_0, float distance_1);

void main()
{
    // Set the control points of the output patch
    c_out[gl_InvocationID].position = c_in[gl_InvocationID].position;
    // v_out.position.y += info.r;
    // v_out.world_position = vec3(u_model * vec4(v_out.position, 1.0));
    // c_out[gl_InvocationID].world_position = c_in[gl_InvocationID].world_position;


    // Calculate the distance from the camera to the three control points
    vec3 camera_position =  u_camera_position;

    float distance_0 = distance(camera_position, vec3(u_model * vec4(c_out[0].position, 1.0)));
    float distance_1 = distance(camera_position, vec3(u_model * vec4(c_out[1].position, 1.0)));
    float distance_2 = distance(camera_position, vec3(u_model * vec4(c_out[2].position, 1.0)));

    // Calculate the tessellation levels
    // gl_TessLevelOuter[0] = getTessLevel(distance_1, distance_2);
    // gl_TessLevelOuter[1] = getTessLevel(distance_2, distance_0);
    // gl_TessLevelOuter[2] = getTessLevel(distance_0, distance_1);
    // gl_TessLevelInner[0] = gl_TessLevelOuter[2];

    // gl_TessLevelOuter[0] = 1.0;
    // gl_TessLevelOuter[1] = 1.0;
    // gl_TessLevelOuter[2] = 1.0;
    // gl_TessLevelInner[0] = 1.0;

    gl_TessLevelOuter[0] = 500000.0 / (distance_1 + distance_2);
    gl_TessLevelOuter[1] = 500000.0 / (distance_2 + distance_0);
    gl_TessLevelOuter[2] = 500000.0 / (distance_0 + distance_1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}

float getTessLevel(float distance_0, float distance_1)
{
    float AvgDistance = (distance_0 + distance_1) / 2.0;
    if (AvgDistance <= 5.0) {
        return 7.0;
    }
    else if (AvgDistance <= 10.0) {
        return 3.0;
    }
    else {
        return 1.0;
    }
} 