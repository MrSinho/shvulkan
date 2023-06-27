#version 460
layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 frag_color;

layout (std140, set = 0, binding = 0) uniform uniformBuffer {
	vec4 light_position;
	vec4 light_color;
} ubo;

void main(){
    vec4 dir = frag_position - ubo.light_position;
    frag_color = vec4(0.01) + vec4(ubo.light_color / dot(dir, dir));
}