#pragma once
#include "GameState.h"
#include "Common.h"
#include "Animation.h"
#include "StartMenu.h"
#include "EventQueue.h"
#include "Border.h"
#include "ScrollingBackground.h"

class MenuGameState :
	public GameState
{
public:
	MenuGameState();
	~MenuGameState();

	virtual void f_run(float dt, sf::RenderWindow& window) override;
	virtual void f_update(float dt, sf::RenderWindow & window)override;
	virtual void f_drawSFML(float dt, sf::RenderWindow & window)override;
	virtual void f_takeInput(float dt, sf::RenderWindow & window) override;

	void f_resetMenu();

private:

		
	Sprite				m_Title;

	Border				m_border;
	Animation			m_startbubble;
	vec					m_TitlePos;
	vec					m_MenuPos;
	ScrollingBackground m_Background1, m_Background2;
	Animation			m_character;
	ptr<StartMenu>		m_startMenu;	

	bool m_takeInput;
};
