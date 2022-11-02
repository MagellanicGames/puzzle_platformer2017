#include "..\include\Collider.h"
#include "..\include\Shader.h"
#include "..\include\Drawing.h"
#include "..\include\Shape.h"
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\mat3x3.hpp>
#include <sstream>


Collider::Collider(vecf pos, vec size,bool setOriginCenter/*=true*/)
	:
	m_pos(pos),
	m_size(size),
	m_collisionsActive(false),
	m_collisionsCenter(setOriginCenter),
	m_collideWithPlayer(false)
{
	
	m_debug_rect.setSize(vecf(m_size));
	if(setOriginCenter)
		m_debug_rect.setOrigin(vecf(m_size.x * .5f, m_size.y * .5f));
	m_debug_rect.setFillColor(sf::Color::Transparent);
	m_debug_rect.setOutlineColor(sf::Color::Yellow);
	m_debug_rect.setOutlineThickness(Dim::OUTLINE_THICKNESS);
}


Collider::~Collider()
{
}

void Collider::debug_draw(sf::RenderWindow& window, sf::Color c)
{	
	if (!m_collideWithPlayer)
		c = sf::Color::Green;
	m_debug_rect.setPosition(vecf(m_pos));
	m_debug_rect.setOutlineColor(c);
	window.draw(m_debug_rect);
}

bool Collider::f_within_bounds(vecf & point)const
{
	if (!m_collisionsActive)
		return false;
	if (m_collisionsCenter)
	{
		if (point.x >= m_pos.x - (m_size.x / 2.0f) && point.x <= m_pos.x + (m_size.x / 2.0f) &&
			point.y >= m_pos.y - (m_size.y / 2.0f) && point.y <= m_pos.y + (m_size.y / 2.0f)) //origin is center
			return true;
		else
			return false;
	}
	else
	{
		if (point.x >= m_pos.x && point.x <= m_pos.x + m_size.x &&
			point.y >= m_pos.y && point.y <= m_pos.y + m_size.y) //origin is topleft
			return true;
		else
			return false;
	}
	
}

void Collider::f_set_size(vec s,bool setOriginCenter/*true*/)
{ 
	m_collisionsCenter = setOriginCenter;
	m_size = s;
	m_debug_rect.setSize(vecf(s));
	if(setOriginCenter)
		m_debug_rect.setOrigin(vecf(m_size.x * .5f, m_size.y * .5f));
}
void Collider::f_set_pos(vecf p)
{ 
	m_pos = p;
	m_debug_rect.setPosition(vecf(p));
}

