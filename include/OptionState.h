#pragma once
#include "GameState.h"

class OptionState : public GameState
{
public:

	OptionState();
	~OptionState();

	virtual void f_run(float dt, sf::RenderWindow& window) override;
	virtual void f_drawSFML(float dt, sf::RenderWindow& window)override;
	virtual void f_takeInput(float dt, sf::RenderWindow & window)override;
private:

	std::vector<std::string>	m_options;
	int							m_index;
};