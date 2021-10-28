layout (location = 0) in vec3 position;

out vec2 coord;

void main() {
    coord = position.xy * 0.5 + 0.5;
    gl_Position = vec4(position.xyz, 1.0);
}