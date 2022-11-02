#include "..\include\Console.h"
#include "..\include\EditState.h"
#include "..\include\EventQueue.h"
#include "..\include\Level.h"

Console::Console(vec pos)
	:
	m_spacing(vec(0.0f,15.0f)),
	m_text_size(20),
	m_max_line_length(89),
	m_log_size(8),
	m_overwrite(false)
{
	m_panel.setFillColor(sf::Color(50, 50, 50, 230));
	m_panel.setPosition(vecf(pos));
	m_panel.setSize(vecf(Dim::RES_X, Dim::RES_Y * 0.3f));
	m_panel.setOutlineColor(sf::Color::Blue);
	m_panel.setOutlineThickness(2.0f);

	for (int i = 0; i < m_log_size; i++)
		m_input_log.push_back(" ");

	m_input_line_position = m_spacing * 20;

}


Console::~Console()
{
}

void Console::f_take_input_and_draw(vec & panel_pos, sf::RenderWindow & window)
{
	f_type_input();
	m_panel.setPosition((vecf)panel_pos);
	window.draw(m_panel);	
	SFML_utils::Text::draw(m_panel.getPosition(), m_text_size, "Console v1.0\n", window);
	SFML_utils::Text::draw((vecf)m_panel.getPosition() + (vecf)m_spacing, m_text_size, "Type 'list' to list commands.\n", window);

	for (unsigned int i = 0; i < m_input_log.size(); ++i)
	{
		SFML_utils::Text::draw((vecf)(m_panel.getPosition() + ((vecf)m_spacing * ((float)i + 4))), m_text_size, m_input_log[i], window);
	}	
	SFML_utils::Text::draw(vecf(m_panel.getPosition()) + vecf(m_input_line_position), m_text_size, ":>" + m_input, window);
}

void Console::f_type_input()
{
	static std::string letters_lower = "abcdefghijklmnopqrstuvwxyz0123456789";
	static std::string letters_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	if (SFML_utils::onKeyPress(sf::Keyboard::Key::Return))
	{
		f_process_input(m_input);
		m_input.clear();
	}

	if (m_input.size() > m_max_line_length)
		return;

	for (unsigned int i = 0; i < letters_lower.size(); ++i)
	{
		if (SFML_utils::onKeyPress(static_cast<sf::Keyboard::Key>(i)))
		{	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			{
				if (i < letters_upper.length())
					m_input.push_back(letters_upper.at(i));
				else
					continue;
			}
			else
			{
				m_input.push_back(letters_lower.at(i));
			}
				
		}
			
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::Key::Space))
		m_input.append(" ");
	
	if (SFML_utils::onKeyPress(sf::Keyboard::Key::BackSpace) && m_input.length() > 0)
		m_input.pop_back();
	
}

void Console::f_process_input(std::string input_string)
{		
	f_log_input(input_string);

	if (input_string.substr(0, 4) == "save" || input_string.substr(0,6) == "save o")
	{
		std::string command;
		if (input_string.substr(0, 6) == "save o")
		{
			command = input_string.substr(0, 6);
			m_overwrite = true;
		}
		else
		{
			command = input_string.substr(0, 4);
			m_overwrite = false;
		}

		if (input_string.length() < 6 || SFML_utils::string_contains(input_string,".,!@~\\/><?][{}£$%^&*()+="))
		{
			f_log_input("Invalid file name, too short or invalid characters.");
			return;
		}

		auto file_name = input_string.substr(command.length() + 1, input_string.length() - (command.length() + 1)) + ".lvl";		
		
		if (SFML_utils::file_exists(file_name) && !m_overwrite)
		{
			f_log_input("Level not saved, file already exists.");
		}
		else
		{
			m_editState->f_saveLevelToFile(file_name);
			f_log_input("Level saved to " + file_name);
			if (m_overwrite)
				f_log_input("File overwritten");
		}

		return;
		
	}

	if (input_string.substr(0, 4) == "load")
	{		
		auto file_name = input_string.substr(5, input_string.length() - 5) + ".lvl";

		if (SFML_utils::file_exists(file_name))
		{
			m_levels.push_back(GameState::f_loadLevelFromFile(file_name));
			m_levelpaths.push_back(file_name);
			EventQueue::f_addEvent(Event::NEWLEVELADDED);			
			f_log_input("Level loaded.");
		}
		else
		{			
			f_log_input("File  " + file_name + " does not exist.  Invalid file or name");
		}
	}

	if (input_string.substr(0, 7) == "restart")
	{
		EventQueue::f_addEvent(Event::RESTARTLEVEL);
		f_log_input("Level restarted.");
	}

	if (input_string.substr(0, 5) == "clear")
	{
		m_editState->f_getLevel()->f_clearTiles();
	}

	if (input_string.substr(0, 4) == "list")
	{
		f_log_input("save filname.");
		f_log_input("save o filename.  Overwrite file.");
		f_log_input("load filename.");
		f_log_input("clear. Clear level.");
		f_log_input("restart.");
		f_log_input("show/hide colliders.");
		f_log_input("set par [x].  Set current level's par.");
	}
	
	if (input_string.substr(0, 14) == "show colliders")
	{
		f_log_input("Showing Tile Colliders.");
		EventQueue::f_addEvent(Event::SHOWCOLLISIONTILES);
	}

	if (input_string.substr(0, 14) == "hide colliders")
	{
		f_log_input("Hiding Tile Colliders.");
		EventQueue::f_addEvent(Event::HIDECOLLISIONTILES);
	}

	if (input_string.substr(0, 7) == "set par")
	{
		std::string newPar_s = input_string.substr(8, input_string.length());
		f_log_input("Changed par to " + newPar_s);
		m_changePar = std::stoi(newPar_s);
		EventQueue::f_addEvent(Event::CHANGEPAR);
	}
	
}

void Console::f_log_input(std::string input)
{
	if (m_input_log.size() == m_log_size)
	{
		m_input_log.erase(m_input_log.begin());
	}
	m_input_log.push_back(input);

}

void Console::f_setEditState(EditState* e)
{
	m_editState = e;
}

std::tuple<std::string, Level*> Console::f_getLastNewLevel()
{
	std::tuple<std::string, Level*> result = std::make_tuple(m_levelpaths[0], m_levels[0]);
	m_levelpaths.clear();
	m_levels.clear();
	return result;
}