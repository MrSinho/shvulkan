#version 460
layout (location = 0) in vec4 frag_position;
layout (location = 0) out vec4 frag_color;

void main(){
    frag_color = vec4(0.5f, 0.0f, 1.0f, 1.0f);
}