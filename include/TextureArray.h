#pragma once
#include "Common.h"

class SubImage
{
public:

	SubImage()
		:
		m_currentPos(0)
	{

	}
	

	~SubImage();

	void insertData(unsigned char* data, int dataLength);

	std::vector<unsigned char> m_data;
	int m_currentPos;
};
class Shader;
class TextureArray
{
public:

	TextureArray(GLuint id = 0);
	TextureArray(const TextureArray & ta);
	~TextureArray();

	void loadFromFile(std::string path,int subImageHeight,int subImageWidth);
	void bindArray(const ptr<Shader> &  shader);
	static void unbind();
	static void unbindArray();

	void cleanup();

	GLuint m_id;

	int m_widthInTiles;
	int m_heightInTiles;
private:

	
};