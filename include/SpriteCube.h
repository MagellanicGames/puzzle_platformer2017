#pragma once
#include "Common.h"
#include "Cubemap.h"
#include "glm\vec3.hpp"


class Shape;
class Shader;

class SpriteCube
{
public:

	SpriteCube(Cubemap c = Cubemap());
	~SpriteCube();

	void f_draw(float dt, const ptr<Shader> & shader);
	void f_setPos(vecf pos,float z);

	void f_setCubeMap(const Cubemap & cubemap) { m_cubemap = cubemap; }

private:

	Cubemap			m_cubemap;
	glm::vec3		m_pos, m_scale;
	Shape*			m_shape;

};