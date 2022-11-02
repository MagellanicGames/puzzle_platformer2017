#include "..\include\TextureArray.h"
#include "..\include\Shader.h"
#include "..\include\stb_image.h"
#include <iostream>

TextureArray::TextureArray(GLuint id /*=0*/)
	:
	m_id(id)
{	
}

TextureArray::TextureArray(const TextureArray & ta)
	:
	m_id(ta.m_id),
	m_widthInTiles(ta.m_widthInTiles),
	m_heightInTiles(ta.m_heightInTiles)
{	
}

TextureArray::~TextureArray()
{}

void TextureArray::cleanup()
{
	glDeleteTextures(1, &m_id);
}

//fetches image dimensions when loading from file using stbi
void TextureArray::loadFromFile(std::string path,int subImageWidth, int subImageHeight)
{

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

	int width, height, components;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, 0);

	m_widthInTiles = width /subImageWidth;
	m_heightInTiles = height / subImageHeight;

	if (!data)
		std::cout << "Err loading " << path << "\n";
	
	//split image into tiles
	std::vector<std::vector<SubImage>> tiles(m_heightInTiles); //outter array is the number or rows

	for (auto & arr : tiles)
		for (int i = 0; i < m_widthInTiles; i++) //fill will inner array equal to width in number of tiles
			arr.push_back(SubImage());

	unsigned char* sub_data = data;

	for (int y = 0; y < m_heightInTiles; y++)
	{
		for (int row = 0; row < subImageHeight; row++) //pixel row, not subimage row
		{
			for (int x = 0; x < m_widthInTiles; x++)
			{

				tiles[y][x].insertData(sub_data, subImageWidth * components);
				sub_data += subImageWidth * components;
			}
		}		
	}	

	glTexImage3D(GL_TEXTURE_2D_ARRAY, //allocate storage
		0, //mipmap level
		GL_RGBA,//format
		subImageWidth,
		subImageHeight,
		m_widthInTiles * m_heightInTiles, 
		0,//border
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		0); 


	int zOffset = 0; //load tiles into gl texture array
	for (int y = 0; y < m_heightInTiles; y++)
	{
		for (int x = 0; x < m_widthInTiles; x++)
		{
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
				0,  //level
				0,//xoffset
				0, //yoffset
				zOffset,//zoffset (index of array)
				subImageWidth, //width
				subImageHeight,//height
				1,//depth 
				GL_RGBA, //format
				GL_UNSIGNED_BYTE,//type
				tiles[y][x].m_data.data());//data
			zOffset++;
		}
	}	


	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	stbi_image_free(data);
}

void TextureArray::unbind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureArray::bindArray(const ptr<Shader> &  shader)
{
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(shader->uniform("textureArray"), 2);
	glUniform1i(shader->uniform("isTex2d"), true);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
}

void TextureArray::unbindArray()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

SubImage::~SubImage() {};

void SubImage::insertData(unsigned char* data ,int dataLength)
{
	m_data.insert(m_data.begin() + m_currentPos, data, data + dataLength);
	m_currentPos += dataLength;
}