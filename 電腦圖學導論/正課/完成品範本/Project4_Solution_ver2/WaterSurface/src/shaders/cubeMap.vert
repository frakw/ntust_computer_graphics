out vec3 texture_pos;

layout (location = 0) in vec3 position;


void main()
{
    mat4 view = mat4(mat3(u_view));    //remove traslate
    vec4 pos = u_projection * view * vec4(position, 1.0);
    
    texture_pos = vec3(position.x, position.y, position.z);
    gl_Position = pos.xyww; //set depth to 1.0(deepest!)
}  