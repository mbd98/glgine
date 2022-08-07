#version 330 core

layout(location = 0) in vec3 a_Position;
uniform mat4 u_Model;
out vec2 pos;
void main( ){
      pos = a_Position.xy;
      gl_Position =  u_Model * vec4(a_Position,1.0);
}