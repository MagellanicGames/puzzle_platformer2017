#pragma once
#include "Common.h"
#include "EventQueue.h"

class GameDialog
{
public:

	GameDialog(std::string titleText = "TitleText");
	virtual ~GameDialog();

	virtual void f_draw(float dt, sf::RenderWindow & window);

	virtual void f_update(float dt, sf::RenderWindow & window);

	virtual void f_activateSelectedOption();

	std::string f_selectedOption();

	void f_resetToFirstOption() { m_currentOptionIndex = 0; }

	static std::string RESUME_STR;
	static std::string RESTART_STR;
	static std::string RETURNOVERWORLD_STR;
	static std::string RETURNMAINMENU_STR;

protected:

	sf::RectangleShape	m_panel;
	vec					m_pos;
	vec					m_size;
	vec					m_titlePos;
	std::string			m_titleText;

	vecf m_firstOptionPos;

	int	m_currentOptionIndex;

	std::vector<std::string> m_options;
	
};