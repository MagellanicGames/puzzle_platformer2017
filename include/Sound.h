#pragma once
#include <SFML\Audio.hpp>

class Sound
{
public:



	static void initialise();

	static sf::Sound menuSelect,jingle,impact03,mainTheme;

private:

	static sf::SoundBuffer menuSelectBuffer,jingleBuffer,impact03Buffer,mainThemeBuffer;
};