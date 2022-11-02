#include "..\include\EditState.h"
#include "..\include\Drawing.h"
#include "..\include\Block.h"
#include "..\include\EventQueue.h"
#include <iostream>


vecf resDiv2(Dim::RES_X / 2, Dim::RES_Y / 2);

EditState::EditState(vec levelDimensions /*=vec()*/)
	:
	m_spriteIndex(0),
	m_hideBorder(false),
	m_viewCenter(vecf(Dim::RES_X / 2,Dim::RES_Y / 2)),
	m_camSpeed(300.0f)
{
	m_console.f_setEditState(this);

	m_currentSprite = Drawing::m_tileset1[m_spriteIndex].get();

	for (int y = 0; y < levelDimensions.y;y++)
	{
		m_dragTiles.push_back(std::vector<ptr<Tile>>());
		for (int x = 0; x < levelDimensions.x; x++)
			m_dragTiles[y].push_back(ptr<Tile>(nullptr));
	}
		

	
	m_playerSprite.setFillColor(sf::Color::Green);
	m_playerSprite.setSize(vecf(4, 4));
	m_playerSprite.setOrigin(vecf(2, 2));

	m_colliderCursor.setSize(vecf(Dim::TILE_SIZE_S));
	m_colliderCursor.setFillColor(sf::Color::Transparent);
	m_colliderCursor.setOutlineColor(sf::Color::Green);
	m_colliderCursor.setOutlineThickness(Dim::OUTLINE_THICKNESS);

	m_subState = &m_editTileState;
}

EditState::~EditState()
{

}

void EditState::f_run(float dt, sf::RenderWindow& window)
{
	static bool test = false;	

	GameState::f_run(dt, window);
		m_subState->f_run(dt, window, this);

	m_view = window.getView();
	m_view.setCenter(m_viewCenter);
	window.setView(m_view);
	m_border.f_setPos(m_viewCenter - resDiv2);
	
	if (!window.hasFocus())
		return;

	if (SFML_utils::onKeyReleased(sf::Keyboard::Key::F2))
		m_showConsole = !m_showConsole;

	if (m_showConsole) //if console is showing, no input elsewhere
		return;

	if (SFML_utils::onKeyPress(sf::Keyboard::T))
	{
		m_subState = &m_editTileState;
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::C))
	{
		m_subState = &m_editColliderState;
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::S))
	{
		m_subState = &m_placeStartState;
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::M))
	{
		m_subState = &m_moveLevelState;
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::B))
	{
		m_subState = &m_placeBlockState;
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::F))
	{
		m_subState = &m_placeFoodState;
	}

	if (sf::Keyboard::isKeyPressed(Controls::RIGHT))
	{
		m_viewCenter.x += m_camSpeed * dt;
	}
	if (sf::Keyboard::isKeyPressed(Controls::LEFT))
	{
		m_viewCenter.x -= m_camSpeed * dt;
	}
	if (sf::Keyboard::isKeyPressed(Controls::UP))
	{
		m_viewCenter.y -= m_camSpeed * dt;
	}
	if (sf::Keyboard::isKeyPressed(Controls::DOWN))
	{
		m_viewCenter.y += m_camSpeed * dt;
	}

}

void EditState::f_updateMouse(sf::RenderWindow & window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	m_mousePos = vec(window.mapPixelToCoords(pixelPos));
}




void EditState::f_drawTileDrag(float dt, sf::RenderWindow& window)
{
	for (auto & a : m_dragTiles)
	{
		for (auto & t : a)
		{
			if (t != nullptr)
				t->f_draw(dt, window);
		}
	}
		
}

void EditState::f_addTileDrag(vecf pos)
{
	if (pos.x < 0.0f || pos.y < 0.0f)
			return;	

	sf::Vector2i arrayPos(pos.x / Dim::TILE_SIZE_S.x, pos.y / Dim::TILE_SIZE_S.y);

	if (arrayPos.x > (m_level->f_levelDimensions().x - 1) || arrayPos.y > (m_level->f_levelDimensions().y - 1))
		return;

	if (m_dragTiles[arrayPos.y][arrayPos.x] == nullptr)
		m_dragTiles[arrayPos.y][arrayPos.x] = ptr<Tile> (new Tile(pos, Dim::TILE_SIZE_S, m_currentSprite,m_spriteIndex));
	
}

void EditState::f_moveDragTiles()
{
	int y = 0;
	int x = 0;
	for (auto & a : m_dragTiles)
	{
		for (auto & t : a)
		{
			if (t != nullptr)
			{
				m_level->f_addTile(t);
				m_dragTiles[y][x] = nullptr;
			}				
			x++;
		}
		y++;
		x = 0;
	}
}

void EditState::f_saveLevelToFile(std::string location) const
{
	SFML_utils::fileout(location, m_level->toString());
}

void EditState::f_drawControls(sf::RenderWindow& window)
{

	SFML_utils::Text::draw( m_viewCenter - vecf(0.0f,resDiv2.y), 30, "Edit Mode", window);
	SFML_utils::Text::draw(m_viewCenter + vecf(-(resDiv2.x),resDiv2.y - Dim::TILE_SIZE_S.y), 25, m_instructions.c_str(), window, sf::Color::Yellow);
	SFML_utils::Text::draw(m_viewCenter - vecf(resDiv2.x / 2, resDiv2.y), 30, "Position " + SFML_utils::vecToString(m_mousePos), window, sf::Color::Cyan);
}

void EditState::f_setLevel(Level* l)
{
	m_level = l;
}