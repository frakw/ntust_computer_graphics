out V_OUT
{
   vec3 position;
} v_out;

layout (location = 0) in vec3 position;

// uniform sampler2D u_water;

void main() {
    v_out.position = position.xzy;
}