#version 330 core

in vec2 texCoord;

uniform sampler2D depthMap;

out vec4 FragColor;

void main()
{
	float depth = texture(depthMap, texCoord).z;
	FragColor = vec4(vec3(depth), 1.0);
}
