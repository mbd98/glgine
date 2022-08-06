#version 330 core

out vec4 color;

uniform vec3 u_Color;


void main(){
//    color = vec4(1.0,1.0,0.0,1.0);
    color = vec4(u_Color, 1.0);
}