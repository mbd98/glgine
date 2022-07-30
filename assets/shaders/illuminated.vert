#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;

out VertexData
{
	vec3 fragNormal;
	vec3 fragPosition;
	vec4 fragLightPosition;
	vec2 texCoord;
} vertexOut;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vertexOut.fragNormal = vec3(model * vec4(normal, 0.0));
	vertexOut.fragPosition = vec3(model * vec4(position, 1.0));
	vertexOut.fragLightPosition = light * vec4(vertexOut.fragPosition, 1.0);
	vertexOut.texCoord = textureCoord;
}
