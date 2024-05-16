#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 projection;

void main(){
    gl_Position = projection * vec4(a_Position.x, a_Position.y, a_Position.z - 20.0f, 1.0);
}
