#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;
in vec4 fragLightPosition;
in vec2 texCoord;

uniform sampler2D textureMap;
uniform sampler2D shadowMap;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambientStrength = 0.1;
uniform float diffuseStrength = 0.6;
uniform float specularStrength = 0.3;
uniform vec3 viewPosition;

out vec4 FragColor;

float calculateShadow(vec4 fragLightPos, float bias)
{
	vec3 proj = fragLightPos.xyz / fragLightPos.w;
	proj = proj * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, proj.xy).r;
	float currentDepth = proj.z;
	return ((currentDepth - bias) < closestDepth) ? 1.0 : 0.0;
}

void main()
{
	vec3 lightDirection = normalize(lightPosition - fragPosition);
    float shadow = calculateShadow(fragLightPosition, 0.003);
	vec3 ambient = ambientStrength * lightColor;
	vec3 diffuse = diffuseStrength * max(dot(normalize(fragNormal), lightDirection), 0.0) * lightColor;
	vec3 viewDirection = normalize(viewPosition - fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normalize(fragNormal));
	vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), 32) * lightColor;
	FragColor = vec4(ambient + shadow * (diffuse + specular), 1.0) * texture(textureMap, texCoord);
}
