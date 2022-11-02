#include "..\include\SpriteCube.h"
#include "..\include\Drawing.h"
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\mat3x3.hpp>
#include "..\include\Shape.h"

SpriteCube::SpriteCube(Cubemap cubemap/*= Cubemap()*/)
	:
	m_cubemap(cubemap),
	m_shape(Drawing::m_cubeShape.get()),
	m_scale(glm::vec3(Dim::TILE_SIZE_S.x,Dim::TILE_SIZE_S.y,Dim::TILE_SIZE_S.y))
{}

SpriteCube::~SpriteCube()
{}

void SpriteCube::f_draw(float dt, const ptr<Shader> & shader)
{
	shader->use();
	glm::mat4 model(1.0f); //identity	
	model = glm::translate(model, m_pos + glm::vec3(Dim::TILE_SIZE_S.x * 0.5f, -Dim::TILE_SIZE_S.y,0.0f));
	//model *= glm::mat4_cast(mRotation);
	model = glm::scale(model, m_scale);

	glUniformMatrix4fv(Drawing::m_shader->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(Drawing::m_shader->uniform("normMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::mat3(glm::inverse(model)))));

	m_cubemap.bind(shader);

		m_shape->draw();

	Cubemap::unbind();
}

void SpriteCube::f_setPos(vecf pos,float z)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = z;
}