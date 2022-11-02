#pragma once
#include <glad\glad.h>
#include <SFML\OpenGL.hpp>
#include <glm\vec3.hpp>
#include <vector>
#include "Vertex.h"
#include <array>
#include <SFML\Graphics.hpp>

class Shape
{
public:

	
	Shape();
	Shape(const std::vector<Vertex> & vertices);
	Shape(const Shape & shape);
	~Shape();

	Shape operator=(const Shape & s) { return Shape(s); }

	void draw( GLenum drawmode = GL_TRIANGLES);

protected:

	GLuint		mVao, mVbo;
	GLuint		m_numVertices;
	const int	POSITION_ATTRIB_LOC = 0;
	const int	NORMAL_ATTRIB_LOC = 1;
	const int	TEXCOORDS_ATTRIB_LOC = 2;
};

