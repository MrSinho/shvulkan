#version 460
layout (location = 0) in vec3 position;

layout(location = 0) out vec4 frag_position;

void main() {
  frag_position =  vec4(position, 1.0);
  gl_Position = frag_position;
}