#version 330 core

in vec2 texCoord;

uniform sampler2D shadowMap;

out vec4 FragColor;

void main()
{
	float depth = texture(shadowMap, texCoord).z;
	FragColor = vec4(vec3(depth), 1.0);
}
