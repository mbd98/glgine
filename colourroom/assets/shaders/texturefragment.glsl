#version 410 core

const float PI = 3.1415926535897932384626433832795;

in vec3 vertexColor;
in vec2 vertexUV;
in vec3 vertexNormal;
in vec3 FragPos;
in vec3 FragNormal;
in vec4 FragPosLightSpace;
uniform sampler2D GlossSampler;
uniform sampler2D CarpetSampler;
uniform sampler2D WoodSampler;
uniform sampler2D CarrotSampler;

uniform sampler2D shadowmap;


uniform vec3 lightposition;
uniform vec3 lightdirection;
uniform vec3 lightcolor;

uniform vec3 lightpositionR;
uniform vec3 lightdirectionR;
uniform vec3 lightcolorR;

uniform vec3 viewposition;
uniform uint colorMode = 0;


uniform float light_cutoff_outer;
uniform float light_cutoff_inner;


out vec4 FragColor;

in vec4 gl_FragCoord;



//adapted from lab08 code
//function that returns 1 if a surface receives light, and 0 if not
float shadow_scalar() {

    vec3 ndc = FragPosLightSpace.xyz / FragPosLightSpace.w;
    ndc = ndc * 0.5 + 0.5;
    float closestdepth = texture(shadowmap, ndc.xy).r;
    float currentdepth = ndc.z;
    float bias = 0.003;
    return ((currentdepth - bias) > closestdepth) ? 1.0 : 0.0;
}

//adapted from lab08 code
//add light direction
float spotlight_scalar(vec3 lpos, vec3 ldir) {
    float theta = dot(normalize(FragPos - lpos), ldir);
    
    if(theta > light_cutoff_inner) {
        return 1.0;
    } else if(theta > light_cutoff_outer) {
        return (1.0 - cos(PI * (theta - light_cutoff_outer) / (light_cutoff_inner - light_cutoff_outer))) / 2.0;
    } else {
        return 0.0;
    }
}

float ambientStrength = 0.3;
float diffuseStrength = 1.0;
float specularStrength = 0.5;

vec3 ambient(vec3 color)
{
	return ambientStrength * color;
}

vec3 diffuse(vec3 color, vec3 lpos)
{
	vec3 ldir = normalize(lpos - FragPos);
	return diffuseStrength * color * max(dot(normalize(FragNormal), ldir),0.0) * color;
}

vec3 specular(vec3 color, vec3 lpos)
{
	vec3 ldir = normalize(lpos - FragPos);
    vec3 viewdirection = normalize(viewposition - FragPos);
    vec3 reflectlightdirection = reflect(-ldir, normalize(FragNormal));
    return specularStrength * color * pow(max(dot(reflectlightdirection, viewdirection), 0.0f),32);
}


void main()
{
	//vec4 textureColor = texture(CarrotSampler, vertexUV);	
	//FragColor = textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
	
	float shadow = 1.0; //shadow_scalar();
	
	
	
	
	vec3 lightdirectionX = normalize(lightposition - FragPos);
	vec3 lightdirectionRX = normalize(lightpositionR - FragPos);
	
	vec3 ambient = ambientStrength * vec3(1.0,1.0,1.0); //ambient(lightcolor); //We exceptionally hardcode the ambient light here since we don't want it to turn off with the light
	vec3 diffuse = diffuse(lightcolor, lightposition);
	vec3 specular = specular(lightcolor, lightposition);
	
	
	vec3 ambientR = vec3(1.0,1.0,1.0); //vec3(0,0,0); //ambient(lightcolorR);
	vec3 diffuseR = diffuse(lightcolorR, lightpositionR);
	vec3 specularR = specular(lightcolorR, lightpositionR);
	
	
	/*
	vec3 viewdirection = normalize(viewposition - FragPos);
	vec3 reflectlightdirection = reflect(-lightdirection,normalize(FragNormal));
	vec3 specular = specularStrength * lightcolor * pow(max(dot(reflectlightdirection, viewdirection), 0.0),32);
	*/
	
	float scalar = shadow * spotlight_scalar(lightposition, lightdirection);		
	float scalarR = 1.0; //shadow * spotlight_scalar(lightpositionR, lightdirectionR);
	
	vec4 textureColor = vec4(1.0,1.0,1.0,1.0);
	
	
	
	if (colorMode == 0)		//Use carpet texture
	{
		textureColor = texture(CarpetSampler, vertexUV);
		FragColor = textureColor * vec4((ambient.x + scalar * diffuse.x + scalar * specular.x), (ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 1.0f); //vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);}
	}
	if (colorMode == 1)		//Use wood texture
	{
		textureColor = texture(WoodSampler, vertexUV);
		FragColor = textureColor * vec4((ambient.x + scalar * diffuse.x + scalar * specular.x), (ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 1.0f); //vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);}
	}
	if (colorMode == 2)		//Use blank colour
	{
		textureColor = vec4(1.0,1.0,1.0,1.0); //texture(GlossSampler, vertexUV);
		FragColor = textureColor * vec4((ambient.x + scalar * diffuse.x + scalar * specular.x), (ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 1.0f); //vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);}
	}
	if (colorMode == 3)		//Use gloss colour
	{
		textureColor = texture(GlossSampler, vertexUV);
		FragColor = textureColor * vec4((ambient.x + scalar * diffuse.x + scalar * specular.x), (ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 1.0f); //vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);}
	}
	if (colorMode == 4)		//Use gloss colour
	{
		textureColor = texture(CarrotSampler, vertexUV);
		FragColor = textureColor * vec4((ambient.x + scalar * diffuse.x + scalar * specular.x), (ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 1.0f); //vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);}
	}
	if (colorMode == 5)		//red lens
	{
		FragColor = vec4(1.0,0.0,0.0,0.2); //vec4((ambient.x + scalar * diffuse.x + scalar * specular.x), 0*(ambient.y + scalar * diffuse.y + scalar * specular.y), 0*(ambient.z + scalar * diffuse.z + scalar * specular.z), 0.5f);
	}
	if (colorMode == 6)		//green lens
	{
		FragColor = vec4(0.0,1.0,0.0,0.2); //vec4(0*(ambient.x + scalar * diffuse.x + scalar * specular.x), 0*(ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 0.5f);
	}
	if (colorMode == 7)		//blue lens
	{
		FragColor = vec4(0.0,0.0,1.0,0.2); //vec4(0*(ambient.x + scalar * diffuse.x + scalar * specular.x), 0*(ambient.y + scalar * diffuse.y + scalar * specular.y), (ambient.z + scalar * diffuse.z + scalar * specular.z), 0.5f);
	}

}
