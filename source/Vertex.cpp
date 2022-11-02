#include "..\include\Vertex.h"


Vertex::Vertex(float xx, float yy, float zz, float nx, float ny, float nz, float u, float v)
	:
	pos(glm::vec3(xx,yy,zz)),
	norm(glm::vec3(nx,ny,nz)),
	uv(u,v)
{
}

Vertex::Vertex(float x, float y, float z, VertexUV uv)
	:
	pos(glm::vec3(x, y, z)),
	norm(glm::vec3(0.0f)),
	uv(uv)
{}

Vertex::Vertex(float x, float y, float z,float nx,float ny,float nz,VertexUV uv)
	:
	pos(glm::vec3(x, y, z)),
	norm(glm::vec3(nx,ny,nz)),
	uv(uv)
{}

VertexUV::VertexUV(float u, float v)
	:
	uv(glm::vec2(u, v))
{}

VertexUV::VertexUV(glm::vec2 uvs)
	:
	uv(uvs)
{}