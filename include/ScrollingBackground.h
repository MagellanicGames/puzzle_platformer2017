#pragma once
#include "Common.h"
class ScrollingBackground
{
public:

	ScrollingBackground(int width, int height, vec pos = vec(0, 0));
	~ScrollingBackground();

	void f_draw(sf::RenderWindow& window);
	void f_update(float dt);

private:

	Sprite			m_Tile;
	int				m_width, m_height;
	vec				m_pos, m_originalpos;
	float			m_scrollspeed;
};