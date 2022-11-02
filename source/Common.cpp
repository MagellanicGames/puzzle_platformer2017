#include "..\include\Common.h"
#include <iostream>
#include "..\include\Framebuffer.h"

int Dim::RES_X = 1920; //render resolution
int Dim::RES_Y = 1080;
int Dim::TEXT_SIZE = 27;
int Dim::TITLE_SIZE = 48;

int Dim::VIEW_RES_X = 1920;
int Dim::VIEW_RES_Y = 1080;

vec Dim::TILE_SIZE(16,16);
vec Dim::SPRITE_SCALE(4, 4);
vec Dim::TILE_SIZE_S;

float Dim::OUTLINE_THICKNESS = 4.0f;

void Dim::initialise()
{
	Dim::TILE_SIZE_S = vec(Dim::TILE_SIZE.x * Dim::SPRITE_SCALE.x, Dim::TILE_SIZE.y * Dim::SPRITE_SCALE.y);
}

sf::Keyboard::Key Controls::UP = sf::Keyboard::Key::Up;
sf::Keyboard::Key Controls::DOWN = sf::Keyboard::Key::Down;
sf::Keyboard::Key Controls::LEFT = sf::Keyboard::Key::Left;
sf::Keyboard::Key Controls::RIGHT = sf::Keyboard::Key::Right;
sf::Keyboard::Key Controls::ACCEPT = sf::Keyboard::Key::Space;
sf::Keyboard::Key Controls:: BLOCK_MOVE = sf::Keyboard::Key::Space;


sf::Image Screen::icon;
bool Screen::windowed = true;

void Screen::initialise()
{
	Screen::icon.loadFromFile("Graphics/icon.png");
}

void Screen::setVideoMode(ptr<sf::RenderWindow> & currentWindow,std::string windowTitle, bool windowed)
{
	sf::ContextSettings settings(24, 8, 4, 3, 3, sf::ContextSettings::Default);
	settings.antialiasingLevel = 0;

	if(currentWindow != nullptr)
		currentWindow->close();
	if(windowed)
		currentWindow.reset(new sf::RenderWindow(sf::VideoMode(Dim::VIEW_RES_X, Dim::VIEW_RES_Y), windowTitle, sf::Style::Titlebar | sf::Style::Close,settings));
	else
		currentWindow.reset(new sf::RenderWindow(sf::VideoMode(Dim::VIEW_RES_X, Dim::VIEW_RES_Y), windowTitle, sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen,settings));

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD\n";		
	}
	
	std::cout << "Maximum OpenGL version supported: " << glGetString(GL_VERSION)
		<< "\nGLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) 
		<< "\nVendor: " << glGetString(GL_VENDOR) 
		<< "\nRenderer: " << glGetString(GL_RENDERER) << "\n\n";

	sf::View view(sf::FloatRect(0.0, 0.0, Dim::RES_X, Dim::RES_Y));
	currentWindow->setView(view);	
	currentWindow->setIcon(Screen::icon.getSize().x, Screen::icon.getSize().y, Screen::icon.getPixelsPtr()); 
	Screen::windowed = windowed;
}

void Screen::changeResolution(ptr<sf::RenderWindow> & window, ptr<Framebuffer> & framebuffer)
{
	window->setSize(sf::Vector2u(Dim::VIEW_RES_X,Dim::VIEW_RES_Y));
	framebuffer.reset(new Framebuffer(Dim::RES_X, Dim::RES_Y, Dim::VIEW_RES_X, Dim::VIEW_RES_Y));
}

float WorldTime::seconds = 0.0f;
int WorldTime::minutes = 0;
int WorldTime::hours = 12;
float WorldTime::factor =1000.0f;

float WorldTime::lightBrightness = 1.0f;
float WorldTime::lightColorFactor = 0.5f;

bool WorldTime::eveningColor = false;

int brightness = 12;

WorldTime::WorldTime()
{}

WorldTime::~WorldTime()
{}

void calculateBrightnessRatio()
{
	int lengthOfPeriod = 12;
	float minRatio = 0.2f;
	float maxRatio = 1.0f;
	float differenceRatio = maxRatio - minRatio;

	float ratioIncrement = ((differenceRatio * 10) / lengthOfPeriod) * 0.1f;
	std::cout << "Calculated \n";
}

void WorldTime::update(float dt)
{
	seconds += dt*factor;
	
	if (seconds > 60.0f)
	{
		float excess = seconds - 60.0f;
		minutes += seconds / 60;
		seconds = excess;
	}

	if (minutes >= 60)
	{
		int excess = minutes - 60;
		hours += minutes / 60;

		if (hours <= 12)
			brightness += 1;
		else
			brightness -= 1;
	
		minutes = excess;

		if (hours >18)
			eveningColor = true;
		else
			eveningColor = false;

		
	
	}

	if (hours > 23)
	{
		int excess = hours - 24;
		hours = excess;
	}
		
	if(hours < 22 && hours > 2)
		lightBrightness = (1.0f / 12.0f) * brightness;

	/*
	8pm-4am purple
	
	midday brightest, midnight darkest

	12 = 1

	*/
}

std::string WorldTime::toString() 
{
	std::string secs_string;
	std::string min_string;
	std::string hour_string;
	if (seconds < 10)
		secs_string = "0" + std::to_string((int)seconds);
	else
		secs_string = std::to_string((int)seconds);

	if (minutes < 10)
		min_string = "0" + std::to_string(minutes);
	else
		min_string = std::to_string(minutes);

	if (hours < 10)
		hour_string = "0" + std::to_string(hours);
	else
		hour_string = std::to_string(hours);

	
	return hour_string + ":" + min_string +":"+ secs_string;
}

