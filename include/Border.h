#pragma once
#include "Entity.h"

class Border : public Entity
{
public:

	Border(vec size, vecf position);
	~Border();

	void f_setPos(vecf & v) override
	{
		m_pos.x = v.x;
		m_pos.y = v.y;
		m_collider.f_set_pos(m_pos + m_collider_pos_correction);
		m_background.setPosition(v + (vecf)Dim::TILE_SIZE_S);
	}

	void f_update(float dt, sf::RenderWindow& window)override;
	void f_draw(float dt, sf::RenderWindow& window)override;

	void setFilled(bool b) {m_filled = b;}
private:

	bool				m_filled;
	sf::RectangleShape	m_background;
};