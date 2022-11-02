#pragma once
#include <glad\glad.h>
#include <SFML\OpenGL.hpp>
#include <string>
#include <map>

class Shader {


public:


	Shader(const char* vertex_path = "Shaders\\shader.vert", const char* frag_path = "Shaders\\shader.frag");

	virtual			~Shader();
	GLuint			program(){ return mShaderProgram; };
	void			use();	
	GLuint			uniform(std::string name);

	
private:
	void				storeUniformLocations();
	void				storeUniformLocation(std::string name);
	std::string			insertNumberToString(std::string start_sub, int num, std::string end_sub);
	std::string			getShaderSource(const char* path);
	void				checkCompileStatus(GLuint shader, const char* shaderName);
	GLuint				compileShader(const char* source, GLenum shaderType);
	GLuint				createShaderProgram(GLuint vert, GLuint frag, GLuint geo = 0, bool attachGeoShader = false, char* fragDataLocation = "outColor");

	GLuint							mVert;
	GLuint							mGeo;
	GLuint							mFrag;
	GLuint							mShaderProgram;
	std::map<std::string, GLuint>	uniforms;	

};

