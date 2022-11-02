#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <glad\glad.h>
#include "..\include\SFML_utils.h"
#include "ScoreKeeper.h"
#include <memory>

#include "Sound.h"

typedef sf::Vector2i vec;
typedef sf::Vector2f vecf;

typedef sf::Vector3i vec3;
typedef sf::Vector3f vec3f;

typedef SFML_utils::Text Text;
typedef sf::Texture Texture;
typedef sf::Sprite Sprite;
template<typename T>
using ptr = std::shared_ptr<T>;

enum Dir{UP,DOWN,LEFT,RIGHT, HORIZONTAL,VERTICAL,TOP,BOTTOM,FRONT,BACK,SAME};

class Framebuffer;

class Dim
{
public:

	static void initialise();

	static int RES_X;
	static int RES_Y;

	static int TEXT_SIZE;
	static int TITLE_SIZE;	

	static vec TILE_SIZE;
	static vec SPRITE_SCALE;

	static vec TILE_SIZE_S;

	static int VIEW_RES_X;
	static int VIEW_RES_Y;

	static float OUTLINE_THICKNESS;
};

class Controls
{
public:

	static sf::Keyboard::Key UP;
	static sf::Keyboard::Key DOWN;
	static sf::Keyboard::Key LEFT;
	static sf::Keyboard::Key RIGHT;

	static sf::Keyboard::Key ACCEPT;
	static sf::Keyboard::Key BLOCK_MOVE;
};
class Screen
{
public:

	static bool windowed;
	static void initialise();
	static void setVideoMode(ptr<sf::RenderWindow> & currentWindow, std::string windowTitle, bool windowed);
	static void changeResolution(ptr<sf::RenderWindow> & window, ptr<Framebuffer> & framebuffer);

	static sf::Image icon;
};

class WorldTime
{
public:

	WorldTime();
	~WorldTime();

	static void update(float dt);

	static int hour(){ return hours; }

	static std::string toString();

	static float	lightBrightness;

	static bool		eveningColor;

private:

	static float	seconds;
	static int		minutes, hours;
	static float	factor;//how much faster than real-time

	
	static float	lightColorFactor;
		


	

};