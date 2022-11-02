#include "..\include\StartMenu.h"
#include "..\include\Drawing.h"
#include "..\include\GameState.h"

StartMenu::StartMenu(vecf pos)
	:
	m_pos(pos),
	m_show(false),
	m_startSelected(true),
	m_quitSelected(false),
	m_box(vec(Dim::TILE_SIZE_S.x * 7, Dim::TILE_SIZE_S.y * 5),pos)
{
	m_box.setFilled(true);
	m_box.f_setPos(pos - (vecf)m_box.f_size() / 2.0f);
	m_startGameText.f_addFrame(Drawing::f_LoadSprite(&Drawing::m_miscSheet, vec(280 - 213, 221 - 208), vec(213, 208), Dim::SPRITE_SCALE));
	m_startGameText.f_addFrame(Drawing::f_LoadSprite(&Drawing::m_miscSheet, vec(280 - 213, 221 - 208), vec(213, 224), Dim::SPRITE_SCALE));
	m_startGameText.f_set_position(m_pos - (m_startGameText.f_size() / 2.0f) - vecf(0, m_startGameText.f_size().y ));

	m_optionText = m_startGameText;
	m_optionText.f_set_position(m_startGameText.f_pos() + vecf(0, m_startGameText.f_size().y));
	m_optionText.f_set_current_frame(1);
		
	m_quitText.f_addFrame(Drawing::f_LoadSprite(&Drawing::m_miscSheet, vec(239 - 213, 252 - 239), vec(213, 239), Dim::SPRITE_SCALE));
	m_quitText.f_addFrame(Drawing::f_LoadSprite(&Drawing::m_miscSheet, vec(239 - 213, 252 - 239), vec(241, 239), Dim::SPRITE_SCALE));
	m_quitText.f_set_current_frame(1);
	m_quitText.f_set_position(m_startGameText.f_pos() + vecf(0, m_startGameText.f_size().y * 2.0f));
}

StartMenu::~StartMenu() {}

void StartMenu::f_update(float dt, sf::RenderWindow& window)
{

	if (m_show)
	{		
		if (m_startSelected)
		{
			if (SFML_utils::onKeyPress(Controls::DOWN))
			{
				m_startGameText.f_set_current_frame(1);
				m_optionText.f_set_current_frame(0);
				m_startSelected = false;
				m_optionsSelected = true;
				Sound::menuSelect.play();
			}
		}

		if (m_optionsSelected)
		{
			if (SFML_utils::onKeyPress(Controls::UP))
			{
				m_optionsSelected = false;
				m_startSelected = true;
				m_startGameText.f_set_current_frame(0);
				m_optionText.f_set_current_frame(1);
				Sound::menuSelect.play();
			}

			if (SFML_utils::onKeyPress(Controls::DOWN))
			{
				m_optionsSelected = false;
				m_quitSelected = true;
				m_optionText.f_set_current_frame(1);
				m_quitText.f_set_current_frame(0);
				Sound::menuSelect.play();
			}			
		}

		if (m_quitSelected)
		{
			if (SFML_utils::onKeyPress(Controls::UP))
			{
				m_optionText.f_set_current_frame(0);
				m_quitText.f_set_current_frame(1);
				m_optionsSelected = true;
				m_quitSelected = false;
				Sound::menuSelect.play();
			}
		}
	}

}

void StartMenu::f_draw(sf::RenderWindow & window)
{
	m_box.f_draw(0.0f, window);
	m_startGameText.f_draw(window);
	m_optionText.f_draw(window);
	m_quitText.f_draw(window);
}