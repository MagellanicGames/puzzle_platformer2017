#include "..\include\Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>



Shader::Shader(const char* vert_path, const char* frag_path)
	:
	mShaderProgram(0)
{
	std::string vert(getShaderSource(vert_path));
	const char* vertex_source = vert.c_str();

	mVert = compileShader(vertex_source, GL_VERTEX_SHADER);
	checkCompileStatus(mVert, vert_path);
	
	std::string frag(getShaderSource(frag_path));
	const char* fragment_source = frag.c_str();

	mFrag = compileShader(fragment_source, GL_FRAGMENT_SHADER);
	checkCompileStatus(mFrag, frag_path);
	
	mShaderProgram = createShaderProgram(mVert, mFrag);
	
	glUseProgram(mShaderProgram);

	storeUniformLocations();

}


std::string Shader::getShaderSource(const char* path)
{
	using namespace std;

	fstream shader_file;
	shader_file.open(path, ios::in | ios::binary);
	if (!shader_file.is_open())
		std::cout << "Failed to open vertex shader location.\n" << std::endl;

	stringstream shader_stream;
	shader_stream << shader_file.rdbuf();

	string result(shader_stream.str());
	return result;
}

void Shader::checkCompileStatus(GLuint shader_id, const char* shaderName)
{
	GLint compiled;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(shader_id, 512, NULL, buffer);

		std::cout << buffer << std::endl;
	}
	else
	{
		std::cout << "Shader " << "'" << shaderName << "'" << " successfully compiled.\n" << std::endl;
	}

}

Shader::~Shader()
{
	glDeleteShader(mVert);
	glDeleteShader(mGeo);
	glDeleteShader(mFrag);
	glDeleteProgram(mShaderProgram);
}

void Shader::use()
{
	glUseProgram(mShaderProgram);
}

GLuint Shader::compileShader(const char* source, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);
	return shaderID;
}

GLuint Shader::createShaderProgram(GLuint vert, GLuint frag, GLuint geo /*= 0*/, bool attachGeoShader/*false*/, char* fragDataLocation/*="outColor"*/)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vert);
	glAttachShader(shaderProgram, frag);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	return shaderProgram;
}

void Shader::storeUniformLocations()
{
	storeUniformLocation("uColor");
	storeUniformLocation("projection");
	storeUniformLocation("view");
	storeUniformLocation("model");
	
	storeUniformLocation("camPos");	

	storeUniformLocation("texture2d");
	storeUniformLocation("cubemap");
	storeUniformLocation("isTex2d");
	storeUniformLocation("textureArray");
	storeUniformLocation("texArrayIndex");
	storeUniformLocation("flipUV");
	storeUniformLocation("colliderColor");
	storeUniformLocation("drawColliders");
	storeUniformLocation("affectedByLight");
	storeUniformLocation("sunColor");
	storeUniformLocation("sunPos");
	storeUniformLocation("normTest");
}

void Shader::storeUniformLocation(std::string name)
{
	uniforms[name] = glGetUniformLocation(mShaderProgram, name.c_str());
}

GLuint Shader::uniform(std::string name)
{
	if (uniforms.find(name) != uniforms.end())
		return uniforms[name];
	else
		return -1;
}
std::string Shader::insertNumberToString(std::string start_sub, int num, std::string end_sub)
{
	std::stringstream ss;
	ss << num;
	ss << end_sub;
	return start_sub + ss.str();
}