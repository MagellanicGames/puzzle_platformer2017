#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{

	float gamma = 1.0;
	vec3 correction = texture(screenTexture, TexCoords).rgb;
	correction = pow(correction,vec3(1.0/gamma));
	color = vec4(correction,1.0f);
}