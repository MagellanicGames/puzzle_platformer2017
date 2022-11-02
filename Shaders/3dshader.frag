#version 330 core

in vec3					TexCoords;
in vec3					fragPosition;
in vec3					Normal;

uniform samplerCube		cubemap;
uniform sampler2DArray	textureArray;

uniform bool			isTex2d;
uniform bool			flipUV;
uniform int				texArrayIndex;

uniform bool			drawColliders;
uniform vec3			colliderColor;

uniform bool			affectedByLight;
uniform vec3			sunColor;
uniform vec3			sunPos;

out vec4				outColor;



void main()
{
	
	if(isTex2d == true)
	{
		vec2 uvs = vec2(TexCoords.x,TexCoords.y);
		if(flipUV == true)
		{
			uvs.x = 1.0f - uvs.x;
		}
		
		outColor = texture(textureArray,vec3(uvs,texArrayIndex)); //workout colour from texture
	
		if(affectedByLight)
		{				
			vec3 lightDirection = normalize(sunPos - vec3(0.0,0.0,0.0));

			float dotProduct = dot(vec3(0.0,1.0,0.0),lightDirection);
			float brightness = max(dotProduct,0.0);

			vec3 ambient;
			if(brightness <= 0.0)
			{
				ambient = (outColor.rgb * 0.4) + vec3(0.05,0.0,0.03); 
			}
			else
			{
				ambient = outColor.rgb * 0.5;
			}
			
			

			vec3 diffuse = brightness * sunColor;

			outColor.rgb = (outColor.rgb * diffuse) + ambient;
		}
		
	}
	else
	{
		outColor = texture(cubemap, TexCoords);
	}

	if(drawColliders == true)
	{
		outColor = vec4(colliderColor,1.0f);
	}
	
}