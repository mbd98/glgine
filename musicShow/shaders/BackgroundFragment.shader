#version 330 core

out vec4 color;

uniform vec3 u_Color;
uniform float time;
in vec2 pos;

void main(){
//    color = vec4(1.0,1.0,0.0,1.0);
//    vec2 p = pos - 1 + 2. * pos;
    vec2 p = pos;
//    float r = abs(sin(cos(time + 3. * p.y) * 2. * p.x + time)) * 0.5;
//    float g = abs(cos(sin(time + 2. * p.x) * 3. * p.y + time)) * 0.5;
//    float b = abs(cos(time)) * 0.1;
    color = vec4(r,g,0.5,1.0);

}