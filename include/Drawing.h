#pragma once
#include "Common.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "OrthoCamera.h"
#include "Cubemap.h"

#include "..\include\TextureArray.h"

class Animation;
class Tile;
/*
2 pixel border
1 pixel between tiles
16^2 pixel tiles
42 pixel palette size
19 betweel palatte and title
title pos: 2 + 16 * 10 + 10 + 42 + 20
*/

class Shape;

class Drawing
{
public:

	Drawing();
	~Drawing();

	static void initialise();

	static Texture	m_spriteSheet;
	static Texture	m_characterSheet;
	static Texture	m_miscSheet;
	static Sprite	f_LoadSpriteTile(Texture* texture,vec tileSize, vec textureLocation, vec scale);

	static Sprite	f_LoadSprite(Texture* texture, vec spriteSize, vec textureLocation, vec scale);
	

	static Sprite	m_BorderTL, m_BorderL, m_BorderBL, m_BorderB, m_BorderBR, m_BorderR, m_BorderTR, m_BorderT;

	static std::vector<ptr<Sprite>> m_tileset1;
	static std::vector<ptr<Sprite>> m_foodSprites;

	static std::vector<Animation> m_animations;

	static sf::Color			m_clearColor;

	static std::vector<sf::Vertex>				m_grid;

	static void Drawing::draw_grid(sf::RenderWindow & window);

	static ptr<Shader> m_shader;
	static ptr<Shader> m_framebuffer_shader;

	static ptr<Framebuffer> m_framebuffer;

	static void glStart();
	static void glEnd();
	static void sfmlStart(sf::RenderWindow& window, bool clear = false);
	static void sfmlEnd(sf::RenderWindow& window);

	static ptr<Shape>			m_cubeShape;
	static ptr<Shape>			m_planeShape;
	static ptr<Shape>			m_planeRightShape;
	static ptr<Shape>			m_planeTopShape;

	static Cubemap				m_cubemap;

	static std::vector<Cubemap> m_cubeset1;

	static TextureArray			m_characterTexArray;
	static TextureArray			m_spritesheetTexArray;
	static TextureArray			m_mapBasesTexArray;
	static TextureArray			m_mapActivesTexArray;

private:

	
};