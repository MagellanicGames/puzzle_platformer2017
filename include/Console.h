#pragma once
#include "Common.h"
#include "Drawing.h"
class Level;
class EditState;

class Console
{
public:
	
	Console(vec pos = vec());
	~Console();
	
	void f_setEditState(EditState* e);
	
	void f_take_input_and_draw(vec & panel_pos, sf::RenderWindow & window);

	std::tuple<std::string, Level*> f_getLastNewLevel();


	int f_getChangedPar()const { return m_changePar; }
private:

	void f_process_input(std::string input_string);
	void f_log_input(std::string input);
	void f_type_input();

	bool						m_overwrite;
	vec							m_spacing;
	vec							m_input_line_position;
	int							m_text_size;
	int							m_max_line_length;
	int							m_log_size;
	sf::RectangleShape			m_panel;
	std::string					m_input;
	std::vector<std::string>	m_input_log;
	EditState*					m_editState;

	std::vector<std::string>	m_levelpaths;
	std::vector<Level*>			m_levels;

	int							m_changePar;
};

