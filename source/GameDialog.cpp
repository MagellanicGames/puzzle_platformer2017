#include "..\include\GameDialog.h"

std::string GameDialog::RESUME_STR = "Resume";
std::string GameDialog::RESTART_STR = "Restart Level";
std::string GameDialog::RETURNOVERWORLD_STR = "Return to Overworld";
std::string GameDialog::RETURNMAINMENU_STR = "Return to Main Menu.";

GameDialog::GameDialog(std::string titleText /*= TitleText*/)
	:	
	m_size(Dim::RES_X / 3, Dim::RES_Y / 3),
	m_pos(Dim::RES_X / 2, Dim::RES_Y / 2),
	m_titleText(titleText)
{
	m_firstOptionPos = vecf(m_size.x * 0.1f, m_size.y * 0.3f);
	m_pos -= m_size / 2;
	m_panel.setSize((vecf)m_size);
	m_panel.setPosition((vecf)m_pos);
	m_panel.setFillColor(sf::Color::Blue);
	m_titlePos = vec(m_size.x * 0.5f, m_size.y * 0.1f);
	
}

GameDialog::~GameDialog()
{}

void GameDialog::f_update(float dt, sf::RenderWindow & window)
{
	if (SFML_utils::onKeyPress(Controls::DOWN))
	{
		if (m_currentOptionIndex < m_options.size() - 1)
			m_currentOptionIndex++;
		else
			m_currentOptionIndex = 0;
		Sound::menuSelect.play();
	}

	if (SFML_utils::onKeyPress(Controls::UP))
	{
		if (m_currentOptionIndex > 0)
			m_currentOptionIndex--;
		else
			m_currentOptionIndex = m_options.size() - 1;
		Sound::menuSelect.play();
	}
}

void GameDialog::f_activateSelectedOption()
{
	if (f_selectedOption() == GameDialog::RESTART_STR)
		EventQueue::f_addEvent(Event::RESTARTLEVEL);
	else if (f_selectedOption() == GameDialog::RETURNOVERWORLD_STR)
		EventQueue::f_addEvent(Event::RETURNOVERWORLD);
	else if (f_selectedOption() == GameDialog::RETURNMAINMENU_STR)
		EventQueue::f_addEvent(Event::MAINMENU);

	Sound::menuSelect.play();
	f_resetToFirstOption();	
}

void GameDialog::f_draw(float dt, sf::RenderWindow & window)
{
	static int titleSize = 40;
	static int textSize = 30;

	static sf::Color textColor = sf::Color::White;
	window.draw(m_panel);
	SFML_utils::Text::draw((vecf)(m_pos + m_titlePos), titleSize, m_titleText, window, sf::Color::White, true);	

	for (int i = 0; i < m_options.size(); i++)
	{
		if (i == m_currentOptionIndex)
			textColor = sf::Color::Red;
		else
			textColor = sf::Color::White;

		SFML_utils::Text::draw((vecf)m_pos + m_firstOptionPos + vecf(0.0f, (m_size.y * 0.1f) * (float)i), textSize, m_options[i], window, textColor);
	}
}

std::string GameDialog::f_selectedOption()
{
	return m_options[m_currentOptionIndex];
}
