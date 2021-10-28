out vec3 fragUV;

in E_OUT
{
   vec3 position;
   vec3 world_position;
} f_in;

void main() 
{
    fragUV = vec3(f_in.position.xz * 0.5 + 0.5, 1.0f);
    // fragUV = vec3(0.0f, 1.0f, 0.0f);
}