#include "..\include\Primitives.h"

std::vector<Vertex> Primitives::cube = {
	Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,VertexUV(0.0,1.0)), //front tl
	Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,VertexUV(0.0,0.0)), //bl
	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,VertexUV(1.0,0.0)),//br
	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,VertexUV(1.0,0.0)),//br
	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,VertexUV(1.0,1.0)),//tr
	Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,VertexUV(0.0,1.0)),//tl

	Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,VertexUV(0.0,1.0)),  //back
	Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,VertexUV(0.0,0.0)),
	Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,VertexUV(1.0,0.0)),
	Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,VertexUV(1.0,0.0)),
	Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,VertexUV(1.0,1.0)),
	Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,VertexUV(0.0,1.0)),

	Vertex(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f), //bottom
	Vertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f),

	Vertex(0.5f, -0.5, -0.5f, 0.0f, -1.0f, 0.0f),
	Vertex(0.5f, -0.5, 0.5f, 0.0f, -1.0f, 0.0f),
	Vertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f),


	Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),//top
	Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
	Vertex(0.5f, 0.5, 0.5f, 0.0f, 1.0f, 0.0f),
	Vertex(0.5f, 0.5, -0.5f, 0.0f, 1.0f, 0.0f),
	Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),

	Vertex(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f),  //left
	Vertex(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f),

	Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f), //right
	Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f)
};

std::vector<Vertex> Primitives::plane = {
	Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,VertexUV(0.0,0.0)),  
	Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,VertexUV(0.0,1.0)),
	Vertex(0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,VertexUV(1.0,1.0)),
	Vertex(0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,VertexUV(1.0,1.0)),
	Vertex(0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,VertexUV(1.0,0.0)),
	Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,VertexUV(0.0,0.0)) 
};

std::vector<Vertex> Primitives::planeRight = {
	Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, VertexUV(0.0,0.0)),
	Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,VertexUV(0.0,1.0)),
	Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, VertexUV(1.0,1.0)),
	Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, VertexUV(1.0,1.0)),
	Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,VertexUV(1.0,0.0)),
	Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,VertexUV(0.0,0.0))
};

std::vector<Vertex> Primitives::planeTop = {
	Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, VertexUV(0.0,0.0)),
	Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,VertexUV(0.0,1.0)),
	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, VertexUV(1.0,1.0)),
	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, VertexUV(1.0,1.0)),
	Vertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,VertexUV(1.0,0.0)),
	Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,VertexUV(0.0,0.0))
};