#include "..\include\Cubemap.h"
#include "..\include\stb_image.h"
#include <vector>
#include <iostream>
#include "..\include\Shader.h"
#include "..\include\SFML_utils.h"

Cubemap::Cubemap()
{
}

Cubemap::Cubemap(std::string file_path)
{
	loadFromImageFile(file_path);
}

Cubemap::Cubemap(const Cubemap & c)
	:
	mId(c.mId)
{}

Cubemap::~Cubemap()
{
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------

void Cubemap::loadFromImageFile(std::string image_file)
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mId);

	int width, height, components;

	unsigned char* data = stbi_load(image_file.c_str(), &width, &height, &components, 0);

	if (!data)	
		std::cout << "Failed to load cubemap file " << image_file << "\n";
	

	std::vector<unsigned char*> images(6);

	int single_tile_length = (width*height*components) / 6;

	for (int i = 0; i < images.size(); i++)
	{
		int start = i * single_tile_length;
		images[i] = data + start;		
	}
	
	for (int i = 0; i < images.size(); i ++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height / 6, 0, GL_RGBA, GL_UNSIGNED_BYTE,images[i]);
		
	}

	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void Cubemap::bind(const ptr<Shader> & shader)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(shader->uniform("cubemap"), 0);
	glUniform1i(shader->uniform("isTex2d"), false);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, mId);
}

void Cubemap::unbind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}