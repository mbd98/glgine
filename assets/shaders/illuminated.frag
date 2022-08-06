#version 330 core

const float PI = 3.1415926535897932384626433832795;

in VertexData
{
	vec3 fragNormal;
	vec3 fragPosition;
	vec4 fragLightPosition;
	vec2 texCoord;
} fragIn;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
uniform sampler2D shadowMap;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float ambientStrength = 0.1;
uniform float diffuseStrength = 0.6;
uniform float specularStrength = 0.3;
uniform vec3 viewPosition;
uniform bool doLighting = true;
uniform float light_cutoff_inner;
uniform float light_cutoff_outer;
uniform float shine = 32;

out vec4 FragColor;

float calculateShadow(vec4 fragLightPos, float bias)
{
	vec3 proj = fragLightPos.xyz / fragLightPos.w;
	proj = proj * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, proj.xy).r;
	float currentDepth = proj.z;
	return ((currentDepth - bias) < closestDepth) ? 1.0 : 0.0;
}

float calculateSpotlight(vec3 ld) {
    float theta = dot(normalize(-lightDirection), ld);

    if(theta > light_cutoff_inner) {
        return 1.0;
    } else if(theta > light_cutoff_outer) {
        return (1.0 - cos(PI * (theta - light_cutoff_outer) / (light_cutoff_inner - light_cutoff_outer))) / 2.0;
    } else {
        return 0.0;
    }
}

void main()
{
	vec3 lightDir = normalize(lightPosition - fragIn.fragPosition);
    float shadow = calculateSpotlight(lightDir) * calculateShadow(fragIn.fragLightPosition, 0.003);
	vec3 ambient = ambientStrength * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 diffuse = diffuseStrength * max(dot(normalize(fragIn.fragNormal), lightDir), 0.0) * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 viewDirection = normalize(viewPosition - fragIn.fragPosition);
	vec3 reflectDirection = reflect(-lightDir, normalize(fragIn.fragNormal));
	vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), shine) * vec3(texture(texture_specular0, fragIn.texCoord));
	if (doLighting)
		FragColor = vec4(ambient + shadow * (diffuse + specular), 1.0);
	else
		FragColor = vec4(ambient, 1.0);
}