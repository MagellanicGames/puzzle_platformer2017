#include "..\include\MenuGameState.h"
#include "..\include\Drawing.h"
#include <iostream>

MenuGameState::MenuGameState()
	:
	m_TitlePos(Dim::RES_X * 0.5, Dim::RES_Y * 0.25),
	m_MenuPos(m_TitlePos + vec(0,30)),
	m_Background1(36,12,vec((Dim::TILE_SIZE.x * Dim::SPRITE_SCALE.x * 2) * 0, Dim::TILE_SIZE.y * Dim::SPRITE_SCALE.y)),
	m_Background2(36,12,vec((Dim::TILE_SIZE.x * Dim::SPRITE_SCALE.x * 2) * 18, Dim::TILE_SIZE.y * Dim::SPRITE_SCALE.y)),
	m_startMenu(new StartMenu(vecf(Dim::RES_X * 0.5f, Dim::RES_Y * 0.6f))),
	m_border(vec(Dim::RES_X,Dim::RES_Y),vecf()),
	m_takeInput(true)
{
	m_Title.setTexture(Drawing::m_miscSheet);
	m_Title.setTextureRect(sf::IntRect(212, 0, 461 - 233, 59));
	m_Title.setScale(vecf(Dim::SPRITE_SCALE.x,Dim::SPRITE_SCALE.y));
	vec size(m_Title.getGlobalBounds().width, m_Title.getGlobalBounds().height);
	vecf titlePos(m_TitlePos.x - (size.x / 2), m_TitlePos.y - (size.y / 2));
	m_Title.setPosition(titlePos);

		
	m_character.f_loadframes(&Drawing::m_characterSheet, 8, 32, 32, 0, 1,true);
	m_character.f_set_scale((vecf)Dim::SPRITE_SCALE);
	m_character.f_set_frametime(0.1f);
	m_character.f_start_animation(true);
	m_character.f_set_position(vecf(Dim::RES_X * 0.5f, Dim::RES_Y * 0.6f));

	vec tileScaled(Dim::TILE_SIZE.x * Dim::SPRITE_SCALE.x, Dim::TILE_SIZE.y * Dim::SPRITE_SCALE.y);

	m_startbubble.f_addFrame(Drawing::f_LoadSprite(&Drawing::m_miscSheet, vec(371 - 291, 104 - 83), vec(291, 83), Dim::SPRITE_SCALE));
	m_startbubble.f_addFrame(Sprite());
	m_startbubble.f_set_frametime(1.0f);
	m_startbubble.f_start_animation(true);
	m_startbubble.f_set_position(m_character.f_pos() + vecf(tileScaled.x * 0.5f,-tileScaled.y));	
}

void MenuGameState::f_resetMenu()
{
	m_startMenu.reset(new StartMenu(vecf(Dim::RES_X * 0.5f, Dim::RES_Y * 0.6f)));
}
MenuGameState::~MenuGameState()
{
	std::cout << "Menu State deleted\n";
}

void MenuGameState::f_run(float dt, sf::RenderWindow& window)
{	

	GameState::f_run(dt, window);
	GameState::f_pollEvents(dt, window);
	f_update(dt, window);
	f_draw(dt, window,true);	
}

void MenuGameState::f_update(float dt, sf::RenderWindow & window)
{
	m_Background1.f_update(dt);
	m_Background2.f_update(dt);

	f_takeInput(dt,window);
		
	if (!m_startMenu->f_show())
	{
		m_character.f_update(dt);
		m_startbubble.f_update(dt);
	}
	if (GameState::m_switchTimer < 0.0f)
		EventQueue::f_addEvent(Event::STARTGAME);
}


void MenuGameState::f_drawSFML(float dt, sf::RenderWindow& window)
{
	m_Background1.f_draw(window);
	m_Background2.f_draw(window);

	m_border.f_draw(0.0f, window);

	window.draw(m_Title);

	if (m_startMenu->f_show())
	{
		m_startMenu->f_draw(window);		
	}
	else
	{
		m_character.f_draw(window);
		m_startbubble.f_draw(window);
	}

	window.draw(m_fadeRect);
}

void MenuGameState::f_takeInput(float dt, sf::RenderWindow & window)
{
	GameState::f_takeInput(dt, window);
	if (!m_windowHasFocus)
		return;
	m_startMenu->f_update(dt, window);

	if (!m_takeInput)
		return;
	if (SFML_utils::onKeyReleased(Controls::ACCEPT))
	{
		
		if (m_startMenu->f_show() == false)
		{
			m_startMenu->f_toggleShow();
		}
		else
		{
			if (m_startMenu->f_startGame())
			{
				GameState::f_fadeOut();
				m_takeInput = false;
				return;
			}
				

			if (m_startMenu->f_openOptions())
			{
				m_takeInput = false;
				EventQueue::f_addEvent(Event::OPENOPTIONS);
			}
				

			if (m_startMenu->f_quitGame())
			{
				m_takeInput = false;
				EventQueue::f_addEvent(Event::QUITGAME);
			}
			
		}

		Sound::menuSelect.play();

	}
	
}