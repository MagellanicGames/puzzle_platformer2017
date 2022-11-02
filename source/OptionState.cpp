#include "..\include\OptionState.h"
#include "..\include\EventQueue.h"
#include "..\include\Drawing.h"

OptionState::OptionState()
{
	m_options.push_back("w640x480h");
	m_options.push_back("w800x600h");
	m_options.push_back("w1024x768h");
	m_options.push_back("w1280x960h");
	m_options.push_back("w1280x1042h");
	m_options.push_back("w1280x720h");
	m_options.push_back("w1280x800h");
	m_options.push_back("w1440x900h");
	m_options.push_back("w1920x1080h");
	m_options.push_back("w1920x1200h");
	m_options.push_back("wFullscreen");
	m_options.push_back("wWindowed");
}

OptionState::~OptionState()
{}

void OptionState::f_run(float dt, sf::RenderWindow& window)
{
	GameState::f_pollEvents(dt, window);
	GameState::f_run(dt, window);
	GameState::f_draw(dt, window, true);
	f_takeInput(dt, window);	
}

void OptionState::f_drawSFML(float dt, sf::RenderWindow & window)
{
	vecf position;
	int i = 0;
	for (auto &s : m_options)
	{
		if (i == m_index)
			Text::draw(position, 56, s.substr(1, s.length() - 1), window, sf::Color::Yellow);
		else
			Text::draw(position, 56, s.substr(1, s.length() - 1), window);

		position.y += 56.0f;
		i++;
	}
}

void OptionState::f_takeInput(float dt, sf::RenderWindow & window) 
{
	GameState::f_takeInput(dt, window);
	if (!m_windowHasFocus)
		return;
	if (SFML_utils::onKeyPress(Controls::UP))
	{
		if (m_index > 0)
			m_index--;
		else
			m_index = m_options.size() - 1;
	}

	if (SFML_utils::onKeyPress(Controls::DOWN))
	{
		if (m_index < m_options.size() - 1)
			m_index++;
		else
			m_index = 0;
	}

	if (SFML_utils::onKeyPress(Controls::ACCEPT))
	{
		if (m_options[m_index] == "wFullscreen")
		{
			EventQueue::f_addEvent(Event::CHANGEFULLSCREEN);
		}
		else if (m_options[m_index] == "wWindowed")
		{
			EventQueue::f_addEvent(Event::CHANGEWINDOWED);
		}
		else
		{
			std::string res_s = m_options[m_index];

			vec dataPos = SFML_utils::string_dataposition("w", "x", res_s);
			std::string width_s = SFML_utils::sub_string(res_s, dataPos);

			dataPos = SFML_utils::string_dataposition("x", "h", res_s);
			std::string height_s = SFML_utils::sub_string(res_s, dataPos);

			Dim::VIEW_RES_X = atoi(width_s.c_str());
			Dim::VIEW_RES_Y = atoi(height_s.c_str());

			EventQueue::f_addEvent(Event::CHANGERES);
		}
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::Escape))
		EventQueue::f_addEvent(Event::MAINMENU);
}