#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 light0;
uniform mat4 light1;

out vec2 texCoord;

void main()
{
	gl_Position = light1 * light0 * model * vec4(position, 1.0);
	texCoord = textureCoord;
}
