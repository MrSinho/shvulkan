#version 460
//per vertex data
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
//per instance data
layout (location = 2) in mat4 model;
//consumes also locations 3 4 and 5

//output to fragment shader
layout (location = 0)      out vec4 frag_position;
layout (location = 1) flat out float instance_factor;

layout (push_constant) uniform pushConstants {
	mat4 projection;
	mat4 view;
} pconst;

void main() {
  instance_factor = 1 / (gl_InstanceIndex + 1.0f);
  frag_position   = model * vec4(position, 1.0f);
  gl_Position     = pconst.projection * pconst.view * frag_position;
}