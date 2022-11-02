#pragma once
#include "Common.h"
#include "TextureArray.h"
#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>
#include "Shape.h"

class DrawableGL
{
public:
	DrawableGL(TextureArray ta = TextureArray(),GLuint subImageIndex = 0);
	virtual ~DrawableGL();

	void f_setTextureArray(TextureArray & ta) { m_textureArray = ta; }

	virtual void f_draw(const ptr<Shader> & shader);
	virtual void f_update(float dt) = 0;

	virtual void f_set_position(const vecf v, float z = 0.0f);
	virtual void f_set_position(glm::vec3 v) { m_position = v; }
	virtual void f_set_scale(vecf scale);
	virtual void f_set_scale(glm::vec3 v) { m_scale = v; }
	virtual void f_set_rotation(float x, float y, float z); //angle in degrees, function converts to radians.


	void f_setAffectedByLight(bool b) { m_affectedByLight = b; }
	void f_set_name(std::string s) { m_name = s; }

	vecf f_pos() { return vecf(m_position.x, m_position.y); }
	glm::vec3 f_posGLM() { return m_position; }

	void f_flipFrames(bool b);

	vecf f_size() const { return vecf(m_scale.x, m_scale.y); }

	std::string f_getName() { return m_name; }

protected:

	std::string					m_name;
	glm::vec3					m_position;
	glm::vec3					m_scale;
	glm::quat					m_rotation;
	Shape*						m_shape;
	TextureArray				m_textureArray;
	Dir							m_facingDirection;
	bool						m_affectedByLight;
	bool						m_flip;
};