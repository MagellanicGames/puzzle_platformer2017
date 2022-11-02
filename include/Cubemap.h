#pragma once
#include "Common.h"
#include<glad\glad.h>
#include <string>

class Shader;

class Cubemap
{
public:
	Cubemap();
	Cubemap(const Cubemap & c);
	Cubemap(std::string filepath);
	~Cubemap();
	
	void loadFromImageFile(std::string filepath);
	void bind(const ptr<Shader> & shader);
	static void unbind();
	GLuint id() { return mId; }

private:

	GLuint mId;
};

