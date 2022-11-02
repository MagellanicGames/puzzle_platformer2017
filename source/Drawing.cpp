#pragma once
#include "..\include\Drawing.h"
#include <iostream>
#include "..\include\Animation.h"
#include "..\include\Shape.h"
#include "..\include\Primitives.h"

Texture Drawing::m_spriteSheet;
Texture Drawing::m_characterSheet;
Texture Drawing::m_miscSheet;

Sprite Drawing::m_BorderTL;
Sprite Drawing::m_BorderL;
Sprite Drawing::m_BorderBL;
Sprite Drawing::m_BorderB;
Sprite Drawing::m_BorderBR;
Sprite Drawing::m_BorderR;
Sprite Drawing::m_BorderTR;
Sprite Drawing::m_BorderT;


std::vector<ptr<Sprite>>	Drawing::m_tileset1;
std::vector<ptr<Sprite>>	Drawing::m_foodSprites;
std::vector<Animation>		Drawing::m_animations;

std::vector<sf::Vertex>		Drawing::m_grid;

sf::Color Drawing::m_clearColor = sf::Color(20, 20, 20);

ptr<Shader>			Drawing::m_shader;
ptr<Shader>			Drawing::m_framebuffer_shader;

ptr<Framebuffer>	Drawing::m_framebuffer;

ptr<Shape>			Drawing::m_cubeShape;
ptr<Shape>			Drawing::m_planeShape;
ptr<Shape>			Drawing::m_planeRightShape;
ptr<Shape>			Drawing::m_planeTopShape;

std::vector<Cubemap> Drawing::m_cubeset1;

TextureArray		Drawing::m_characterTexArray;
TextureArray		Drawing::m_spritesheetTexArray;
TextureArray		Drawing::m_mapBasesTexArray;
TextureArray		Drawing::m_mapActivesTexArray;

Drawing::Drawing()
{}

Drawing::~Drawing()
{}

void Drawing::initialise()
{		
	Drawing::m_framebuffer_shader.reset(new Shader("Shaders/framebuffer.vert", "Shaders/framebuffer.frag"));
	Drawing::m_shader.reset(new Shader("Shaders/3dshader.vert", "Shaders/3dshader.frag"));

	Drawing::m_characterTexArray.loadFromFile("Graphics/characterSheet.png", 32, 32);
	Drawing::m_spritesheetTexArray.loadFromFile("Graphics/tileset-v0.5.png", 16, 16);
	Drawing::m_mapBasesTexArray.loadFromFile("Graphics/map-bases.png", 160, 160);
	Drawing::m_mapActivesTexArray.loadFromFile("Graphics/map-actives.png", 16, 16);


	Drawing::m_framebuffer.reset(new Framebuffer(Dim::RES_X, Dim::RES_Y, Dim::VIEW_RES_X, Dim::VIEW_RES_Y));

	Drawing::m_cubeShape.reset(new Shape(Primitives::cube));
	Drawing::m_planeShape.reset(new Shape(Primitives::plane));
	Drawing::m_planeRightShape.reset(new Shape(Primitives::planeRight));
	Drawing::m_planeTopShape.reset(new Shape(Primitives::planeTop));

	for (int i = 0; i < 18; i++)
		Drawing::m_cubeset1.push_back(Cubemap("Graphics/Cubemaps/" + std::to_string(i) + ".png"));
	

	if (Drawing::m_spriteSheet.loadFromFile("Graphics/tileset-v0.5.png"))
		std::cout << "Spritesheet loaded\n";
	else
		std::cout << "Failed to load spritesheet.\n";

	if (Drawing::m_characterSheet.loadFromFile("Graphics/characterSheet.png"))
		std::cout << "character sheet loaded\n";
	else
		std::cout << "Failed to load character sheet.\n";

	if(Drawing::m_miscSheet.loadFromFile("Graphics/spriteSheet.png"))
		std::cout << "miscsheet loaded\n";
	else
		std::cout << "Failed to load misc sheet.\n";
												//spritesheet				size					texcutre location				scale
	Drawing::m_BorderT = Drawing::f_LoadSprite(&Drawing::m_spriteSheet, vec(1, Dim::TILE_SIZE.y), vec(Dim::TILE_SIZE.x, 8 * Dim::TILE_SIZE.y), Dim::SPRITE_SCALE);
	Drawing::m_BorderB = Drawing::f_LoadSprite(&Drawing::m_spriteSheet, vec(1, Dim::TILE_SIZE.y), vec(6 * Dim::TILE_SIZE.x, 8 * Dim::TILE_SIZE.y), Dim::SPRITE_SCALE);
	Drawing::m_BorderL = Drawing::f_LoadSprite(&Drawing::m_spriteSheet, vec(Dim::TILE_SIZE.x,1), vec(3 * Dim::TILE_SIZE.x, 8 * Dim::TILE_SIZE.y), Dim::SPRITE_SCALE);	
	Drawing::m_BorderR = Drawing::f_LoadSprite(&Drawing::m_spriteSheet, vec(Dim::TILE_SIZE.x,1), vec(4 * Dim::TILE_SIZE.x, 8 * Dim::TILE_SIZE.y), Dim::SPRITE_SCALE);	
	


	Drawing::m_BorderTL = Drawing::f_LoadSpriteTile(&Drawing::m_spriteSheet, Dim::TILE_SIZE, vec(0, 8), Dim::SPRITE_SCALE);
	Drawing::m_BorderBL = Drawing::f_LoadSpriteTile(&Drawing::m_spriteSheet, Dim::TILE_SIZE, vec(5, 8), Dim::SPRITE_SCALE);
	Drawing::m_BorderTR = Drawing::f_LoadSpriteTile(&Drawing::m_spriteSheet, Dim::TILE_SIZE, vec(2, 8), Dim::SPRITE_SCALE);
	Drawing::m_BorderBR = Drawing::f_LoadSpriteTile(&Drawing::m_spriteSheet, Dim::TILE_SIZE, vec(7, 8), Dim::SPRITE_SCALE);
		
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			m_tileset1.push_back(ptr<Sprite>(new Sprite(Drawing::f_LoadSpriteTile(&Drawing::m_spriteSheet, Dim::TILE_SIZE, vec(x, y), Dim::SPRITE_SCALE))));
		}
	}

	for (int i = 0; i < 2; i++)
		m_tileset1.erase(m_tileset1.begin());

	int startPos = 8;
	for (int i = startPos; i < startPos + 6; i++)
	{
		Drawing::m_foodSprites.push_back(ptr<Sprite>(new Sprite(Drawing::f_LoadSpriteTile(&m_spriteSheet, Dim::TILE_SIZE, vec(i, 0), Dim::SPRITE_SCALE))));
	}
	

	Animation block1(AnimationName::GRNBLOCK1);
	block1.f_set_frametime(0.1f);
	block1.f_loadframes(&m_spriteSheet, 6, 16, 16, 8, 2, true);
	block1.f_start_animation(true);
	block1.f_set_current_frame(5);
	block1.f_set_scale((vecf)Dim::SPRITE_SCALE);
	m_animations.push_back(block1);

	Animation block2(AnimationName::GRNBLOCK2);
	block2.f_set_frametime(0.1f);
	block2.f_loadframes(&m_spriteSheet, 6, 16, 16, 8, 3, true);
	block2.f_start_animation(true);	
	block2.f_set_scale((vecf)Dim::SPRITE_SCALE);
	
	m_animations.push_back(block2);

	Animation flipBlock(AnimationName::FLIPBLOCK);
	flipBlock.f_set_frametime(0.1f);
	flipBlock.f_loadframes(&m_spriteSheet, 8, 16, 16, 8, 1, true);
	flipBlock.f_set_scale((vecf)Dim::SPRITE_SCALE);

	m_animations.push_back(flipBlock);

	int grid_size = 200;
	Drawing::m_grid.resize(grid_size * grid_size * 8);

	vecf correction(0, 0);

	for (int y = 0; y < grid_size; y++)
	{
		for (int x = 0; x < grid_size; x++)
		{
			sf::Vertex* quad = &m_grid[(y + x * grid_size) * 8];

			quad[0].position = vecf(x * 64, y * 64) + correction;
			quad[0].color = sf::Color(70, 70, 70);
			quad[1].position = vecf(x * 64, (y * 64) + 64) + correction;
			quad[1].color = sf::Color(70, 70, 70);
			quad[2].position = vecf(x * 64, (y * 64) + 64) + correction;
			quad[2].color = sf::Color(70, 70, 70);
			quad[3].position = vecf((x * 64) + 64, (y * 64) + 64) + correction;
			quad[3].color = sf::Color(70, 70, 70);

			quad[4].position = vecf((x * 64) + 64, (y * 64) + 64) + correction;
			quad[4].color = sf::Color(70, 70, 70);
			quad[5].position = vecf((x * 64) + 64, y * 64) + correction;
			quad[5].color = sf::Color(70, 70, 70);

			quad[6].position = vecf((x * 64) + 64, y * 64) + correction;
			quad[6].color = sf::Color(70, 70, 70);
			quad[7].position = vecf(x * 64, y * 64) + correction;
			quad[7].color = sf::Color(70, 70, 70);
		}
	}	
}

Sprite Drawing::f_LoadSpriteTile(Texture* texture, vec tileSize, vec textureLocation, vec scale)
{
	Sprite sprite(*texture);
	sprite.setTextureRect(sf::IntRect(tileSize.x * textureLocation.x, tileSize.y * textureLocation.y, tileSize.x, tileSize.y));
	sprite.setScale(vecf(scale));
	return sprite;
}

Sprite Drawing::f_LoadSprite(Texture* texture, vec spriteSize, vec textureLocation, vec scale)
{
	Sprite sprite(*texture);
	sprite.setTextureRect(sf::IntRect(textureLocation.x, textureLocation.y, spriteSize.x, spriteSize.y));
	sprite.setScale(vecf(scale));
	return sprite;
}

void Drawing::draw_grid(sf::RenderWindow & window)
{
	window.draw(Drawing::m_grid.data(), Drawing::m_grid.size(), sf::Lines);
}

void Drawing::glStart()
{
	Drawing::m_framebuffer->bind();
		glViewport(0, 0, Dim::RES_X, Dim::RES_Y);
		float clear = 1.0f / 255.0f * 20.0f;
		glClearColor(clear, clear, clear, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Drawing::glEnd()
{
	Drawing::m_framebuffer->display(Drawing::m_framebuffer_shader.get());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // !! this one as well
	glBindVertexArray(0); // this one as well
}

 void Drawing::sfmlStart(sf::RenderWindow& window, bool clear /*=false*/)
{
	if (clear)
		window.clear(Drawing::m_clearColor);
	window.pushGLStates();
	window.resetGLStates();
}
 void Drawing::sfmlEnd(sf::RenderWindow& window)
{
	
	window.popGLStates();
	window.display();
}