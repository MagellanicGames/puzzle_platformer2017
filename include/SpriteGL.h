#pragma once
#include "Common.h"
#include "DrawableGL.h"

class Shape;
class SpriteGL : public DrawableGL
{
public:

	SpriteGL(TextureArray ta = TextureArray(),GLuint subImageIndex = 0);
	~SpriteGL();	
	
	void f_draw(const ptr<Shader> & shader) override;
	void f_update(float dt) override {}
	void f_setBackfaceCull(bool b) { m_backfaceCull = b; }
	void f_setSubImageIndex(unsigned int index) { m_subImageIndex = index; }
	GLuint f_subImageIndex()const { return m_subImageIndex; }

private:

	bool			m_backfaceCull;
	GLuint			m_subImageIndex;

};