#include "..\include\DrawableGL.h"
#include "..\include\Shape.h"
#include "..\include\Drawing.h"
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\mat3x3.hpp>
#include <iostream>

DrawableGL::DrawableGL(TextureArray ta /*= TextureArray()*/, GLuint subImageIndex /*=0*/)
	:
	m_textureArray(ta),
	m_scale(Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y, Dim::TILE_SIZE_S.y),
	m_shape(Drawing::m_planeShape.get()),	
	m_rotation(glm::quat(glm::vec3())),
	m_affectedByLight(false),
	m_flip(false)
{}

DrawableGL::~DrawableGL()
{}

void DrawableGL::f_draw(const ptr<Shader> & shader)
{	
	glUniform1i(Drawing::m_shader->uniform("flipUV"), m_flip);
	glUniform1i(shader->uniform("drawColliders"), false);
	glUniform3fv(shader->uniform("colliderColor"), 1, glm::value_ptr(glm::vec3(1, 1, 0)));
	glUniform1i(shader->uniform("affectedByLight"), m_affectedByLight);

	m_textureArray.bindArray(shader);
	m_shape->draw();
	TextureArray::unbindArray();
}

void DrawableGL::f_set_position(const vecf v, float z)
{
	m_position.x = v.x;
	m_position.y = v.y;
	m_position.z = z;
}

void DrawableGL::f_set_rotation(float x, float y, float z)
{
	m_rotation *= glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
}

void DrawableGL::f_set_scale(vecf scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
}

void DrawableGL::f_flipFrames(bool b)
{
	m_flip = b;
}