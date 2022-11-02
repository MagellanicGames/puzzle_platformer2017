#include "..\include\Time.h"
#include <math.h>

Time::Time()
{}

void Time::restart()
{
	deltaTime = m_clock.restart().asSeconds();
}