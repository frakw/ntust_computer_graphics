layout (location = 0) in vec3 position;
// layout (location = 1) in vec2 texture_pos;

out V_OUT
{
    vec2 uv;
} v_out;

void main()
{
    gl_Position = vec4(position, 1.0f);
    // v_out.texture_pos = texture_pos;
    v_out.uv = position.xy * 0.5f + 0.5f;
}