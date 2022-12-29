#version 460
//per vertex data
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uv;
//per instance data
layout (location = 3) in mat4 model;

//output to fragment shader
layout (location = 0) out vec4 frag_position;

layout (push_constant) uniform pushConstants {
	mat4 projection;
	mat4 view;
} pconst;

void main() {

  frag_position =  model * vec4(position, 1.0);
  gl_Position = pconst.projection * pconst.view * frag_position;
}