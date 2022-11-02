#pragma once
#include "Common.h"
#include "Time.h"
#include "Shader.h"

class Level;
class GameState
{
public:
	GameState();
	virtual ~GameState();

	virtual void f_run(float dt, sf::RenderWindow& window);

	virtual void f_pollEvents(float dt, sf::RenderWindow & window);
	virtual void f_update(float dt, sf::RenderWindow & window);
	virtual void f_draw(float dt, sf::RenderWindow & window,bool clearOnSfmlStart = false);
	virtual void f_takeInput(float dt, sf::RenderWindow & window);
	virtual void f_drawSFML(float dt, sf::RenderWindow & window);
	virtual void f_drawGL(float dt, ptr<Shader> & shader);

	static Level* f_loadLevelFromFile(std::string fileLocation);
	

protected:


	
	bool f_fadeOutFinished();
	void f_fadeOut();

	sf::Color			m_fadeColor;
	sf::RectangleShape	m_fadeRect;

	float				m_switchTimer;
	bool				m_fadeOut;
	bool				m_takeInput;

	float				m_alpha;

	bool				m_windowHasFocus;

};

