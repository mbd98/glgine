#version 410 core

in vec3 vertexColor;





uniform uint colorMode = 0;
out vec4 FragColor;

void main()
{


	if (colorMode == 0)
	{
       FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
	}
	if (colorMode == 1)		//Tint everything black (use for hair and eyes)
	{
       FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	if (colorMode == 2)		//Tint everything red (use for nose)
	{
       FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
}