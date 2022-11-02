#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normMat;

uniform bool isTex2d;

out vec3 Normal;
out vec3 TexCoords;
out vec3 fragPosition;



void main()
{
	gl_Position		= projection * view * model * vec4(position,1.0f);
	fragPosition	= vec3(model * vec4(position,1.0f));
	Normal			= normalize(normMat * normal);

	if(isTex2d == true)
	{
		TexCoords.xy = texCoords;
	}
	else
	{
		TexCoords = position;
	}
	
}