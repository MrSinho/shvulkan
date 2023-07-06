#version 460
layout (location = 0) in vec4  frag_position;
layout (location = 1) in float instance_factor;

layout (location = 0) out vec4 frag_color;

layout (std140, set = 0, binding = 0) uniform uniformBuffer {
	vec4 light_position;
	vec4 light_color;
} ubo;

void main(){
    vec4 dir     = frag_position - ubo.light_position;
    vec3 result  = vec3(0.01) + vec3(normalize(vec3(instance_factor, ubo.light_color.yz)) / dot(dir, dir));
    frag_color   = vec4(result, 0.5);
}