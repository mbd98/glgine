#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 light;

out vec2 texCoord;

void main()
{
	gl_Position = light * model * vec4(position, 1.0);
	texCoord = textureCoord;
}
