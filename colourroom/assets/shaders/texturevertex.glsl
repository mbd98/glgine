#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 lightspacematrix;

out vec3 vertexColor;
out vec2 vertexUV;
out vec3 vertexNormal;
out vec3 FragPos;
out vec3 FragNormal;
out vec4 FragPosLightSpace;

void main()
{
    vertexColor = aColor;
	mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
	gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexUV = aUV;
	//vertexNormal = aNormal;
	FragNormal = mat3(worldMatrix) * aNormal;
	FragPos = vec3(worldMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0));
	FragPosLightSpace = lightspacematrix * vec4(FragPos, 1.0);
	vertexNormal = aNormal;
}
