layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
    mat4 u_view_inverse;
};
uniform mat4 u_model;

#define u_camera_position u_view_inverse[3].xyz;
#define u_camera_forward u_view_inverse[2].xyz;
