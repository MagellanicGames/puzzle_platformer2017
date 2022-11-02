#pragma once
#include "Common.h"

class Shader;

class Collider
{
public:
	Collider(vecf pos = vecf(0,0), vec size = vec(1,1),bool setOriginCenter = true);
	~Collider();

	bool f_within_bounds(vecf & point) const;

	void f_set_size(vec s,bool setOriginCenter = true);
	void f_set_pos(vecf p);
	void f_setActive(bool b) { m_collisionsActive = b; m_collideWithPlayer = b; }
	void f_setActiveForPlayer(bool b) { m_collideWithPlayer = b; }
	bool f_isActive()const { return m_collisionsActive; }
	bool f_isActiveForPlayer()const { return m_collideWithPlayer; }
	void debug_draw(sf::RenderWindow& window, sf::Color = sf::Color::Yellow);
	vecf& f_pos() { return m_pos; }
	vec f_size() { return m_size; }

private:

	vecf				m_pos;
	vec					m_size;
	sf::RectangleShape	m_debug_rect;
	bool				m_collisionsActive;
	bool				m_collisionsCenter;
	bool				m_collideWithPlayer;
};

