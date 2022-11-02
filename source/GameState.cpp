#include "..\include\GameState.h"
#include "..\include\Common.h"
#include "..\include\Level.h"
#include "..\include\Drawing.h"
#include "..\include\Block.h"
#include "..\include\FlipBlock.h"
#include <iostream>


GameState::GameState()
	:
	m_fadeOut(false),
	m_switchTimer(0.0f),
	m_alpha(1.0f),
	m_takeInput(true),
	m_windowHasFocus(false)
{
	m_fadeColor = sf::Color(0, 0, 0, 255);
	m_fadeRect.setSize(vecf(Dim::RES_X, Dim::RES_Y));
	m_fadeRect.setFillColor(m_fadeColor);
}


GameState::~GameState()
{
}

void GameState::f_run(float dt, sf::RenderWindow& window)
{
	if (!m_fadeOut)
	{
		if (m_fadeColor.a > 1)
		{
			m_alpha -= dt;
			m_alpha = m_alpha > 1.0f ? 1.0f : m_alpha; //ensure within normalised range 0-1
			m_alpha = m_alpha < 0.0f ? 0.0f : m_alpha;

			sf::Uint16 al = m_alpha * 254;
			
			if (al > 254)
			{
				al = 254;
			}
			m_fadeColor.a = al;
			m_fadeRect.setFillColor(m_fadeColor);
		}
	}
	else
	{		
		m_switchTimer -= dt;

		if (m_fadeColor.a < 254)
		{
			m_alpha += dt;
			m_alpha = m_alpha > 1.0f ? 1.0f : m_alpha;//ensure within normalised range 0-1
			m_alpha = m_alpha < 0.0f ? 0.0f : m_alpha;
			m_fadeColor.a = m_alpha * 254;
			m_fadeRect.setFillColor(m_fadeColor);
		}
	}
	
}

void GameState::f_pollEvents(float dt, sf::RenderWindow & window)
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		else if (event.type == sf::Event::Resized)
		{
			glViewport(0, 0, event.size.width, event.size.height);
		}
	}
}
void GameState::f_update(float dt, sf::RenderWindow & window)
{

}
void GameState::f_draw(float dt, sf::RenderWindow & window, bool clearOnSfmlStart/*=false*/)
{
	Drawing::glStart();
		f_drawGL(dt, Drawing::m_shader);
	Drawing::glEnd();

	Drawing::sfmlStart(window, clearOnSfmlStart);
		f_drawSFML(dt, window);
	Drawing::sfmlEnd(window);
}

void GameState::f_drawGL(float dt, ptr<Shader> & shader)
{

}

void GameState::f_drawSFML(float dt, sf::RenderWindow & window)
{

}

void GameState::f_takeInput(float dt, sf::RenderWindow & window)
{
	if (!window.hasFocus())
		m_windowHasFocus = false;
	else
		m_windowHasFocus = true;
}

void GameState::f_fadeOut()
{
	m_alpha = 0.0f;
	m_fadeOut = true;
	m_fadeColor.a = 0;
	m_switchTimer = 1.0f;
	m_takeInput = false;
}

bool GameState::f_fadeOutFinished()
{
	if (m_fadeOut && m_fadeColor.a == 254)
		return true;
	else
		return false;
}



Level* GameState::f_loadLevelFromFile(std::string location)
{
	std::vector<std::string>* levelData = SFML_utils::filein(location);
	Level* level = new Level;

	int i = 1;
	while (levelData->at(i) != "$endOfTiles")
	{
		sf::Vector2i stringPos = SFML_utils::string_dataposition("position:", "spriteIndex", levelData->at(i));
		vecf position = SFML_utils::vecfFromString(SFML_utils::sub_string(levelData->at(i), stringPos));

		stringPos = SFML_utils::string_dataposition("spriteIndex:", ",colliderActive", levelData->at(i));

		std::string spriteIndex_s = SFML_utils::sub_string(levelData->at(i), stringPos);
		unsigned int spriteIndex = atoi(spriteIndex_s.c_str());

		ptr<Tile> t(new Tile(position, Dim::TILE_SIZE_S, Drawing::m_tileset1[spriteIndex].get(), spriteIndex));

		bool collierActive = false;

		stringPos = SFML_utils::string_dataposition("colliderActive:", "}$", levelData->at(i));
		std::string colliderActive_s = SFML_utils::sub_string(levelData->at(i), stringPos);

		if (colliderActive_s == "true")
			t->f_collider().f_setActive(true);
		if (colliderActive_s == "false")
			t->f_collider().f_setActive(false);

		level->f_addTile(t);
		i++;
	}

	i++;
	std::string startPosition_s = SFML_utils::sub_string(levelData->at(i), SFML_utils::string_dataposition("startPos:", "$", levelData->at(levelData->size() - 2)));
	level->f_setStartPos(SFML_utils::vecfFromString(startPosition_s));

	i++;

	if (levelData->at(i) == "$endOfFile}")
		return level;
	else
		i++;

	while (levelData->at(i) != "$endOfCollisionTiles")
	{
		sf::Vector2i stringPos = SFML_utils::string_dataposition("position:", "spriteIndex", levelData->at(i));
		vecf position = SFML_utils::vecfFromString(SFML_utils::sub_string(levelData->at(i), stringPos));

		ptr<Tile> t(new Tile(position, Dim::TILE_SIZE_S, &Drawing::m_BorderB, 0));

		level->f_addCollisionTile(t);
		i++;
	}
	i++;
	while (levelData->at(i) != "$endOfBlocks")
	{
		sf::Vector2i stringPos = SFML_utils::string_dataposition("position:", ",animation", levelData->at(i));
		vecf position = SFML_utils::vecfFromString(SFML_utils::sub_string(levelData->at(i), stringPos));

		stringPos = SFML_utils::string_dataposition("animation:", "$}", levelData->at(i));

		std::string anim_s = SFML_utils::sub_string(levelData->at(i), stringPos);
		int anim = std::stoi(anim_s);
		ptr<Block> b(new Block(position, Drawing::m_animations[static_cast<AnimationName>(anim)], level));
		level->f_addBlock(b);
		i++;
	}

	i++;
	while (levelData->at(i) != "$endOfFood")
	{
		sf::Vector2i stringPos = SFML_utils::string_dataposition("position:", "$}", levelData->at(i));
		vecf position = SFML_utils::vecfFromString(SFML_utils::sub_string(levelData->at(i), stringPos));

		stringPos = SFML_utils::string_dataposition("spriteIndex:", "}$", levelData->at(i));

		std::string sprite_s = SFML_utils::sub_string(levelData->at(i), stringPos);
		int sprite = std::stoi(sprite_s);

		ptr<FoodItem> f(new FoodItem(position, Drawing::m_foodSprites[sprite].get(), sprite));
		level->f_addFoodItem(f);
		i++;
	}
	i++;

	while (levelData->at(i) != "$endOfFlipBlocks")
	{
		sf::Vector2i stringPos = SFML_utils::string_dataposition("position:", ",animation", levelData->at(i));
		vecf position = SFML_utils::vecfFromString(SFML_utils::sub_string(levelData->at(i), stringPos));

		stringPos = SFML_utils::string_dataposition("animation:", "$}", levelData->at(i));

		std::string anim_s = SFML_utils::sub_string(levelData->at(i), stringPos);
		int anim = std::stoi(anim_s);
		ptr<FlipBlock> b(new FlipBlock(position, level));
		level->f_addFlipBlock(b);
		i++;
	}
	i++;
	sf::Vector2i stringPos = SFML_utils::string_dataposition("par{", "}", levelData->at(i));
	std::string par_s = SFML_utils::sub_string(levelData->at(i), stringPos);
	level->f_setPar(std::stoi(par_s));



	return level;
}