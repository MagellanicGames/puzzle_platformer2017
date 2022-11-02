#include "..\include\SpriteGL.h"
#include "..\include\Drawing.h"
#include "..\include\Shape.h"
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\mat3x3.hpp>

SpriteGL::SpriteGL(TextureArray ta /*= TextureArray()*/,GLuint subImageIndex /*=0*/)
	:
	DrawableGL(ta,subImageIndex),
	m_subImageIndex(subImageIndex),
	
	m_backfaceCull(true)
{}

SpriteGL::~SpriteGL()
{}

void SpriteGL::f_draw(const ptr<Shader> & shader)
{	
	shader->use();
	glm::mat4 model(1.0f); //identity
	model = glm::translate(model, m_position);
	model *= glm::mat4_cast(m_rotation);
	model = glm::scale(model, m_scale);
	if (!m_backfaceCull)
		glDisable(GL_CULL_FACE);

	glUniformMatrix4fv(Drawing::m_shader->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glm::mat3 normMat = glm::transpose(glm::mat3(glm::inverse(model)));
	glUniformMatrix3fv(Drawing::m_shader->uniform("normMat"), 1, GL_FALSE, glm::value_ptr(normMat));
	glUniform1i(Drawing::m_shader->uniform("texArrayIndex"), m_subImageIndex);

	DrawableGL::f_draw(shader);

	if (!m_backfaceCull)
		glEnable(GL_CULL_FACE);
}