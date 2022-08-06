#version 330 core

const float PI = 3.1415926535897932384626433832795;

// https://learnopengl.com/Lighting/Multiple-lights
struct PointLight
{
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VertexData
{
	vec3 fragNormal;
	vec3 fragPosition;
	vec4 fragLightPositions[2];
	vec2 texCoord;
} fragIn;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
uniform sampler2D shadowMap;
uniform vec3 lightColor;
uniform PointLight lights[2];
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

float calculateSpotlight(vec3 ld, vec3 ldf) {
    float theta = dot(normalize(-ld), ldf);

    if(theta > light_cutoff_inner) {
        return 1.0;
    } else if(theta > light_cutoff_outer) {
        return (1.0 - cos(PI * (theta - light_cutoff_outer) / (light_cutoff_inner - light_cutoff_outer))) / 2.0;
    } else {
        return 0.0;
    }
}

// https://learnopengl.com/Lighting/Multiple-lights
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec4 fragLightPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shine);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular0, fragIn.texCoord));
	return ambient + calculateSpotlight(lightDir, light.direction) * calculateShadow(fragLightPos, 0.003) * (diffuse + specular);
}

void main()
{
	/*
	vec3 light0Dir = normalize(light0Position - fragIn.fragPosition);
	vec3 light1Dir = normalize(light1Position - fragIn.fragPosition);
    float shadow0 = calculateSpotlight(light0Direction, light0Dir) * calculateShadow(fragIn.fragLight0Position, 0.003);
    float shadow1 = calculateSpotlight(light1Direction, light1Dir) * calculateShadow(fragIn.fragLight1Position, 0.003);
	vec3 ambient = ambientStrength * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 diffuse0 = diffuseStrength * max(dot(normalize(fragIn.fragNormal), light0Dir), 0.0) * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 diffuse1 = diffuseStrength * max(dot(normalize(fragIn.fragNormal), light1Dir), 0.0) * vec3(texture(texture_diffuse0, fragIn.texCoord));
	vec3 viewDirection = normalize(viewPosition - fragIn.fragPosition);
	vec3 reflect0Direction = reflect(-light0Dir, normalize(fragIn.fragNormal));
	vec3 reflect1Direction = reflect(-light1Dir, normalize(fragIn.fragNormal));
	vec3 specular0 = specularStrength * pow(max(dot(viewDirection, reflect0Direction), 0.0), shine) * vec3(texture(texture_specular0, fragIn.texCoord));
	vec3 specular1 = specularStrength * pow(max(dot(viewDirection, reflect1Direction), 0.0), shine) * vec3(texture(texture_specular0, fragIn.texCoord));
	if (doLighting)
		FragColor = vec4(ambient + shadow0 * (diffuse0 + specular0) + shadow1 * (diffuse1 + specular1), 1.0);
	else
		FragColor = vec4(ambient, 1.0);
	*/
	vec3 norm = normalize(fragIn.fragNormal);
	vec3 viewDir = normalize(viewPosition - fragIn.fragPosition);
	vec3 result;
	for (int i = 0; i < 2; i++)
		result += calcPointLight(lights[i], norm, fragIn.fragPosition, fragIn.fragLightPositions[i], viewDir);
	FragColor = vec4(result, 1.0);
}
