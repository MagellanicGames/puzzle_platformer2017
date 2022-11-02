#pragma once
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

struct VertexUV
{
	VertexUV(float u = 0.0f, float v = 0.0f);
	VertexUV(glm::vec2 uv);
	glm::vec2 uv;

	VertexUV operator=(const VertexUV & v)
	{
		return VertexUV(v.uv.x, v.uv.y);
	}

};

struct Vertex
{
	Vertex(float x= 0.0f, float y = 0.0f, float z= 0.0f, float nx = 1.0f, float ny= 1.0f, float nz= 1.0f,float u=0.0f, float v=0.0f);
	Vertex(float x , float y , float z , float nx , float ny , float nz, VertexUV uv);
	Vertex(float x, float y, float z, VertexUV uv);
	glm::vec3 pos;
	glm::vec3 norm;
	VertexUV	uv;
	
	void operator=(const VertexUV & v)
	{
		uv.uv.x = v.uv.x;
		uv.uv.y = v.uv.y;
	}
};





