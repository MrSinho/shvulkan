#version 460
//per instance data
layout (location = 2) in mat4 model;
//consumes also locations 3 4 and 5

//output to fragment shader
layout (location = 0) out vec4 frag_position;

layout (push_constant) uniform pushConstants {
	mat4 projection;
	mat4 view;
} pconst;

vec4 canvas[6] = vec4[6](
	vec4(-1.0f,-1.0f, 0.0f, 1.0f),
	vec4( 1.0f, 1.0f, 0.0f, 1.0f),
	vec4(-1.0f, 1.0f, 0.0f, 1.0f),

	vec4( 1.0f, 1.0f, 0.0f, 1.0f),
	vec4(-1.0f,-1.0f, 0.0f, 1.0f),
	vec4( 1.0f,-1.0f, 0.0f, 1.0f)
);

/*
BRUH I NEED TEXTURES
*/
void main() {

  uint region_index = gl_VertexIndex / 6;

	vec4 vertex = canvas[region_index];

  frag_position = model * vertex;
  gl_Position   = pconst.projection * pconst.view * frag_position;
}