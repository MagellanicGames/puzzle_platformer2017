#include "..\include\Sound.h"

sf::SoundBuffer Sound::menuSelectBuffer;
sf::SoundBuffer Sound::jingleBuffer;
sf::SoundBuffer Sound::impact03Buffer;
sf::SoundBuffer Sound::mainThemeBuffer;
sf::Sound		Sound::menuSelect;
sf::Sound		Sound::jingle;
sf::Sound		Sound::impact03;
sf::Sound		Sound::mainTheme;

void Sound::initialise()
{
	Sound::menuSelectBuffer.loadFromFile("Audio/sfx_impact_04.ogg");
	Sound::menuSelect.setBuffer(Sound::menuSelectBuffer);

	Sound::jingleBuffer.loadFromFile("Audio/sfx_jingle_00.ogg");
	Sound::jingle.setBuffer(Sound::jingleBuffer);

	Sound::impact03Buffer.loadFromFile("Audio/sfx_impact_03.ogg");
	Sound::impact03.setBuffer(impact03Buffer);

	Sound::mainThemeBuffer.loadFromFile("Audio/bg_main_theme.ogg");
	Sound::mainTheme.setBuffer(mainThemeBuffer);

}