#pragma once
#include "GameState.h"
#include "Border.h"

class LevelStartGameState : public GameState
{
public:

	LevelStartGameState(Sprite* levelImage);
	~LevelStartGameState();

	virtual void f_run(float dt, sf::RenderWindow& window) override;

	void f_update(float dt, sf::RenderWindow& window);
	void f_draw(float dt, sf::RenderWindow& window);

private:

	Border	m_border;
	Sprite* m_levelImage;
	float	m_timeout;
	float	m_timer;
};