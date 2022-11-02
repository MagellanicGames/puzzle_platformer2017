#pragma once
#include "GameState.h"
#include "Border.h"
#include "Level.h"
#include "Player.h"

class PlayState : public GameState
{
public:

	PlayState();
	~PlayState();


	virtual void f_run(float dt, sf::RenderWindow& window) override;

	virtual void f_update(float dt, sf::RenderWindow& window);
	virtual void f_draw(float dt, sf::RenderWindow& window);

	virtual void	f_setLevel(Level* l);
	Level*	f_getLevel() { return m_level; }


protected:

	Border		m_border;
	Level*		m_level;
	Player		m_player;
};