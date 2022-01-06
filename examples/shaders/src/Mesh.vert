#version 460
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 0) out vec4 frag_position;

layout (push_constant) uniform pushConstants {
	mat4 projection;
	mat4 view;
} pconst;

void main() {
  frag_position = vec4(position, 1.0);
  gl_Position = pconst.projection * pconst.view * frag_position;
}