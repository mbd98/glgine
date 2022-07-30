#version 330 core

const float PI = 3.1415926535897932384626433832795;

in VertexData
{
	vec3 fragNormal;
	vec3 fragPosition;
	vec4 fragLightPosition;
	vec2 texCoord;
} fragIn;

uniform sampler2D textureMap;
uniform sampler2D shadowMap;
uniform vec4 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambientStrength = 0.1;
uniform float diffuseStrength = 0.6;
uniform float specularStrength = 0.3;
uniform vec3 viewPosition;
uniform bool texturePresent = true;
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
    float theta = dot(normalize(fragIn.fragPosition - lightPosition), ld);

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
	vec3 lightDirection = normalize(lightPosition - fragIn.fragPosition);
    float shadow = /*calculateSpotlight(lightDirection) */ calculateShadow(fragIn.fragLightPosition, 0.003);
	vec3 ambient = ambientStrength * lightColor;
	vec3 diffuse = diffuseStrength * max(dot(normalize(fragIn.fragNormal), lightDirection), 0.0) * lightColor;
	vec3 viewDirection = normalize(viewPosition - fragIn.fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normalize(fragIn.fragNormal));
	vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), shine) * lightColor;
	vec4 color;
	if (texturePresent)
		color = texture(textureMap, fragIn.texCoord);
	else
		color = objectColor;
	if (color.a < 0.01)
		discard;
	if (doLighting)
		FragColor = vec4(ambient + shadow * (diffuse + specular), 1.0) * color;
	else
		FragColor = color;
}
