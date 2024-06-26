#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 projection;
uniform mat4 model;

void main(){
    gl_Position = projection * model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}
