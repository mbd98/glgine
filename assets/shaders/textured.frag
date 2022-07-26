#version 330 core

in vec2 texCoord;

uniform sampler2D textureMap;

out vec4 FragColor;

void main()
{
	FragColor = texture(textureMap, texCoord);
	//FragColor = vec4(vertexColor, 1.0);
}
