#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;

out vec3 fragNormal;
out vec3 fragPosition;
out vec4 fragLightPosition;
out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	fragNormal = vec3(model * vec4(normal, 0.0));
	fragPosition = vec3(model * vec4(position, 1.0));
	fragLightPosition = light * vec4(fragPosition, 1.0);
	texCoord = textureCoord;
}
