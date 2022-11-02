#include "..\include\PlayState.h"
#include "..\include\Drawing.h"

PlayState::PlayState()
	:
	m_border(vec(Dim::RES_X, Dim::RES_Y - Dim::TILE_SIZE_S.y), vecf()),
	m_player(vecf())
{	
}

PlayState::~PlayState()
{}

void PlayState::f_run(float dt, sf::RenderWindow & window)
{
	GameState::f_run(dt, window);

		f_update(dt, window);
		f_draw(dt, window);



}

void PlayState::f_update(float dt, sf::RenderWindow& window)
{
	m_player.f_update(dt, window);
	m_level->f_update(dt, window);
}

void PlayState::f_draw(float dt, sf::RenderWindow& window)
{
	window.clear(Drawing::m_clearColor);
	m_level->f_draw(dt, window);
	m_player.f_draw(dt, window);
	m_border.f_draw(dt, window);
	window.draw(m_fadeRect);
	window.display();
}

void PlayState::f_setLevel(Level* l)
{
	if (m_level == l)
		return;
	else
	{
		m_level = l;
		m_player.f_setLevel(m_level);
		m_player.f_setPos(m_level->f_getStartPos());
		m_player.f_reset();
	}
}