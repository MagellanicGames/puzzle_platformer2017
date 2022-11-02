#include "..\include\OverworldPlayer.h"
#include "..\include\SpriteGL.h"
#include "..\include\Shader.h"
#include "..\include\Drawing.h"
#include "..\include\Overworld.h"

OverworldPlayer::OverworldPlayer(glm::vec3 pos /*=0,0,0*/)	
{
	m_playerSprite.reset(new SpriteGL(Drawing::m_characterTexArray, 62));
	m_playerSprite->f_set_position(pos);
	m_playerSprite->f_setAffectedByLight(true);
}

OverworldPlayer::~OverworldPlayer()
{}

void OverworldPlayer::draw(ptr<Shader> & shader)
{
	m_playerSprite->f_draw(shader);
}

void OverworldPlayer::update(float dt)
{
	static float moveSpeed = 400.0f;

	if (m_playerSprite->f_posGLM().x < m_targetPos.x)
	{
		m_playerSprite->f_set_position(m_playerSprite->f_posGLM() + glm::vec3(moveSpeed*dt, 0.0f, 0.0f));
	}

	if (m_playerSprite->f_posGLM().x > m_targetPos.x)
	{
		m_playerSprite->f_set_position(m_playerSprite->f_posGLM() - glm::vec3(moveSpeed*dt, 0.0f, 0.0f));
	}

	if (m_playerSprite->f_posGLM().z < m_targetPos.z)
	{
		m_playerSprite->f_set_position(m_playerSprite->f_posGLM() + glm::vec3(0.0f, 0.0f, moveSpeed*dt));
	}

	if (m_playerSprite->f_posGLM().z > m_targetPos.z)
	{
		m_playerSprite->f_set_position(m_playerSprite->f_posGLM() - glm::vec3(0.0f, 0.0f, moveSpeed*dt));
	}	


	updateIfMoving(dt);
}

void OverworldPlayer::updateIfMoving(float dt) //store position every other frame and compare with current to determine if moving.
{
	static bool everyOtherFrame = false;
	everyOtherFrame = !everyOtherFrame;

	static vec pos;

	if (everyOtherFrame)
	{
		pos.x = m_playerSprite->f_posGLM().x;
		pos.y = m_playerSprite->f_posGLM().z;
	}
	else
	{
		if (pos.x == (int)m_playerSprite->f_posGLM().x && pos.y == (int)m_playerSprite->f_posGLM().z)
			m_moving = false;
		else
			m_moving = true;
	}
}

void OverworldPlayer::setPos(glm::vec3 v)
{
	m_playerSprite->f_set_position(v);
	m_targetPos = v;
}

glm::vec3 OverworldPlayer::pos()
{ 
	return m_playerSprite->f_posGLM();
}