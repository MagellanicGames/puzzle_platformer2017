#pragma once
#include <SFML\System.hpp>

class Time
{
public:
	Time();	

	
	void restart();

	float deltaTime;

private:

	sf::Clock	m_clock;
	sf::Time	m_time;
};