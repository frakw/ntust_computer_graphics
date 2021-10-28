out vec4 f_color;

in vec3 texture_pos;

void main()
{
    f_color = texture(u_material.texture_cubemap, texture_pos);
    //for hdr
    // f_color.rgb = f_color.rgb * 5.0;
    //f_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}