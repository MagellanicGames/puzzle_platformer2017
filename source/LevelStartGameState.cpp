#include "..\include\LevelStartGameState.h"
#include "..\include\EventQueue.h"
#include "..\include\Drawing.h"
#include <iostream>

LevelStartGameState::LevelStartGameState(Sprite* levelImage)
	:
	m_levelImage(levelImage),
	m_border(vec(Dim::RES_X * 0.6f,Dim::RES_Y * 0.6f), vecf(Dim::TILE_SIZE_S.x * 5.25, Dim::TILE_SIZE_S.x * 2.5)),
	m_timeout(5.0f),
	m_timer(m_timeout)
{

}

LevelStartGameState::~LevelStartGameState()
{}

void LevelStartGameState::f_run(float dt, sf::RenderWindow& window)
{
	GameState::f_run(dt, window);
	f_update(dt, window);

	if (SFML_utils::onKeyPress(sf::Keyboard::Key::Space) && m_timer > 0.0f)
		m_timer = -1.0f;



	f_draw(dt, window);
		
}

void LevelStartGameState::f_update(float dt, sf::RenderWindow& window)
{
	if (m_timer > 0.0f)
		m_timer -= dt;
	else	
		if (!GameState::m_fadeOut)		
			f_fadeOut();			
	

	if(GameState::f_fadeOutFinished())
		EventQueue::f_addEvent(Event::STARTLEVEL);
		
}

void LevelStartGameState::f_draw(float dt, sf::RenderWindow& window)
{
	Drawing::sfmlStart(window, true);
		if (m_levelImage != nullptr)
			window.draw(*m_levelImage);
		else
			std::cout << "No image set on start level\n";

		m_border.f_draw(dt, window);
		window.draw(m_fadeRect);
	Drawing::sfmlEnd(window);
}