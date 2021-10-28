layout (location = 0) in vec3 position;

out V_OUT
{
   vec3 position;
} v_out;

void main() 
{
    v_out.position = position.xyz;
    v_out.position.y = ((1.0 - v_out.position.y) * (7.0 / 12.0) - 1.0) * poolHeight;
    // v_out.position = vec3(u_model * vec4(v_out.position, 1.0f));
    gl_Position = u_projection * u_view * u_model * vec4(v_out.position, 1.0f);
}