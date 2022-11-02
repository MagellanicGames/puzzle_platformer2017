#include "..\include\ScrollingBackground.h"
#include "..\include\Drawing.h"

ScrollingBackground::ScrollingBackground(int width, int height, vec pos /*= 0,0*/)
	:
	m_width(width),
	m_height(height),
	m_pos(pos),
	m_originalpos(pos),
	m_scrollspeed(200.0f)
{
	m_Tile.setTexture(Drawing::m_spriteSheet);
	m_Tile.setTextureRect(sf::IntRect(408, 79, 32, 32));
	m_Tile.setScale(vecf(Dim::SPRITE_SCALE.x,Dim::SPRITE_SCALE.y));
}

ScrollingBackground::~ScrollingBackground()
{}

void ScrollingBackground::f_update(float dt)
{
	m_pos.x -= m_scrollspeed * dt;
	if (m_pos.x < 0.0f - ((Dim::TILE_SIZE.x * 2) * Dim::SPRITE_SCALE.x) * m_width)
		m_pos.x = m_originalpos.x;
}
void ScrollingBackground::f_draw(sf::RenderWindow& window)
{

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			m_Tile.setPosition(vecf((int)(x * Dim::TILE_SIZE.x * Dim::SPRITE_SCALE.x * 2), (int)(y * Dim::TILE_SIZE.y * Dim::SPRITE_SCALE.y * 2)) + vecf(m_pos.x,m_pos.y));
			window.draw(m_Tile);
		}
	}
}



