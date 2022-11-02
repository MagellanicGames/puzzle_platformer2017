#pragma once
#include "Common.h"
#include "Animation.h"
#include "Border.h"

class StartMenu
{
public:

	StartMenu(vecf pos);
	~StartMenu();

	void f_update(float dt, sf::RenderWindow& window);
	void f_draw(sf::RenderWindow& window);
	void f_toggleShow() { m_show = !m_show;}
	bool f_show()const { return m_show; }
	bool f_startGame()const { return m_startSelected; }
	bool f_openOptions()const { return m_optionsSelected; }
	bool f_quitGame()const { return m_quitSelected; }

	vecf					m_pos;

private:

	Border				m_box;
	Animation			m_startGameText;
	Animation			m_optionText;
	Animation			m_quitText;
	bool				m_show;
	bool				m_startSelected;
	bool				m_optionsSelected;
	bool				m_quitSelected;
};