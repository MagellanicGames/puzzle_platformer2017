#include "..\include\Shape.h"
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include "..\include\Drawing.h"


Shape::Shape()	
{}

Shape::Shape(const std::vector<Vertex> & vertices)
	:
	m_numVertices(vertices.size())
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(POSITION_ATTRIB_LOC, sizeof(vertices[0].pos.length()), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(POSITION_ATTRIB_LOC);

	glVertexAttribPointer(NORMAL_ATTRIB_LOC, sizeof(vertices[0].norm.length()), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, norm));
	glEnableVertexAttribArray(NORMAL_ATTRIB_LOC);

	glVertexAttribPointer(TEXCOORDS_ATTRIB_LOC, sizeof(vertices[0].uv.uv.length()), GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(TEXCOORDS_ATTRIB_LOC);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Shape::Shape(const Shape & shape)
	:
	POSITION_ATTRIB_LOC(shape.POSITION_ATTRIB_LOC),	
	NORMAL_ATTRIB_LOC(shape.NORMAL_ATTRIB_LOC),
	TEXCOORDS_ATTRIB_LOC(shape.TEXCOORDS_ATTRIB_LOC),
	m_numVertices(shape.m_numVertices),
	mVao(shape.mVao),
	mVbo(shape.mVbo)
{
}

void Shape::draw(GLenum drawmode /*GL_TRIANGLES*/)
{	
	glBindVertexArray(mVao);		
	glDrawArrays(drawmode, 0, m_numVertices);
	glBindVertexArray(0);		
}

Shape::~Shape()
{
}