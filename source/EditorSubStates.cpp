#include "..\include\EditorSubStates.h"
#include "..\include\EditState.h"
#include "..\include\Block.h"
#include "..\include\FlipBlock.h"
#include <iostream>



EditTileState::EditTileState()
	:
	m_tilepalette(vecf(Dim::RES_X,Dim::TILE_SIZE_S.y) - vecf(Dim::TILE_SIZE_S.x * 6, 0), vec(Dim::TILE_SIZE_S.x * 6, Dim::TILE_SIZE_S.y * 15))
{}

void EditTileState::f_run(float dt, sf::RenderWindow & window, EditState* editState)
{
	sf::Event event;//polled to stop window from becoming unresponsive
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0)
		{
			std::cout << "Scroll Up\n";
			if (editState->m_spriteIndex < Drawing::m_tileset1.size() - 1)
				editState->m_spriteIndex++;
			else
				editState->m_spriteIndex = 0;

			std::cout << "SpriteIndex: " << std::to_string(editState->m_spriteIndex) << "\n";
			editState->m_currentSprite = Drawing::m_tileset1[editState->m_spriteIndex].get();
		}

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0)
		{
			std::cout << "Scroll Down\n";
			if (editState->m_spriteIndex > 0)
				editState->m_spriteIndex--;
			else
				editState->m_spriteIndex = Drawing::m_tileset1.size() - 1;
			std::cout << "SpriteIndex: " << std::to_string(editState->m_spriteIndex) << "\n";
			editState->m_currentSprite = Drawing::m_tileset1[editState->m_spriteIndex].get();
		}
	}
	//update===================================================================================
	editState->f_updateMouse(window);
	editState->m_mousePos = SFML_utils::quantiseVec(editState->m_mousePos, Dim::TILE_SIZE_S.x);
	vecf startPos(editState->m_level->f_getStartPos());
	editState->m_playerSprite.setPosition(startPos);
	m_tilepalette.f_setPos(editState->m_view.getCenter() + vecf(m_tilepalette.m_size.x * 1.5f,-( m_tilepalette.m_size.y * 0.5f)));
	//draw=======================================================================================
	
	Drawing::sfmlStart(window,true);
	editState->m_level->f_draw(dt, window);
	
	window.draw(editState->m_playerSprite);
	editState->f_drawTileDrag(dt, window);
	if (!editState->m_hideBorder)
		editState->m_border.f_draw(dt, window);

	m_tilepalette.f_draw(window);
	
	Drawing::draw_grid(window);
	
	editState->f_drawControls(window);	
	

	if (editState->m_showConsole)
	{
		editState->m_console.f_take_input_and_draw(vec(), window);
	}
	editState->m_currentSprite->setPosition(vecf(editState->m_mousePos));
	window.draw(*editState->m_currentSprite);
	Drawing::sfmlEnd(window);
	//input======================================================================================

	if (!window.hasFocus())
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !paletteActiveAndWithinBounds(editState->m_mousePos))
	{
		editState->f_addTileDrag(vecf(editState->m_mousePos));
	}

	if (SFML_utils::onMouseButtonReleased(sf::Mouse::Button::Left))
	{
		editState->f_moveDragTiles();
	}

	if (SFML_utils::onMouseButtonReleased(sf::Mouse::Button::Middle))
	{
		if (editState->m_level->f_getTile(editState->m_mousePos) != nullptr && !m_tilepalette.m_collider.f_within_bounds((vecf)editState->m_mousePos))
		{
			editState->m_currentSprite = editState->m_level->f_getTile(editState->m_mousePos)->f_getSprite();
			editState->m_spriteIndex = editState->m_level->f_getTile(editState->m_mousePos)->f_getSpriteIndex();
		}

		
		m_tilepalette.f_input((vecf)editState->m_mousePos, window, editState);	
		
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !paletteActiveAndWithinBounds(editState->m_mousePos))
	{
		editState->m_level->f_deleteTile(editState->m_mousePos);
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::P) && !editState->m_showConsole)
	{
		m_tilepalette.m_isShowing = !m_tilepalette.m_isShowing;
	}
}

bool EditTileState::paletteActiveAndWithinBounds(vec mousePos)
{
	if (!m_tilepalette.m_isShowing)
		return false;

	if (m_tilepalette.m_collider.f_within_bounds((vecf)mousePos))
		return true;
	else
		return false;
}

void EditColliderState::f_run(float dt, sf::RenderWindow & window, EditState* editState)
{
	static bool collideWithPlayer = true;
	static sf::Color cursorColor = sf::Color::Yellow;

	sf::Event event;//polled to stop window from becoming unresponsive
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0)
		{
			std::cout << "Scroll Up\n";
			collideWithPlayer = true;
		}

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0)
		{
			std::cout << "Scroll Down\n";
			collideWithPlayer = false;
		}
	}

	if (collideWithPlayer)
		cursorColor = sf::Color::Yellow;
	else
		cursorColor = sf::Color::Green;

	Drawing::sfmlStart(window,true);
	Drawing::draw_grid(window);
	editState->m_level->f_draw(dt, window,true);

	editState->f_updateMouse(window);
	editState->m_mousePos = SFML_utils::quantiseVec(editState->m_mousePos, Dim::TILE_SIZE_S.x);
	
	editState->m_colliderCursor.setPosition(vecf(editState->m_mousePos));
	editState->m_colliderCursor.setOutlineColor(cursorColor);

	

	window.draw(editState->m_playerSprite);
	window.draw(editState->m_colliderCursor);
	if (!editState->m_hideBorder)
		editState->m_border.f_draw(dt, window);
	editState->f_drawControls(window);

	if (!window.hasFocus())
		return;

	if (editState->m_showConsole)
	{
		editState->m_console.f_take_input_and_draw(vec(), window);
	}
	Drawing::sfmlEnd(window);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		editState->m_level->f_addCollisionTile(ptr<Tile> (new Tile(vecf(editState->m_mousePos), Dim::TILE_SIZE_S, nullptr, unsigned int(0))),collideWithPlayer);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (editState->m_level->f_isCollisionTile((vecf)editState->m_mousePos))
			editState->m_level->f_deleteCollisionTile(editState->m_mousePos);
	}
}

void PlaceStartState::f_run(float dt, sf::RenderWindow & window, EditState* editState)
{
	sf::Event event;//polled to stop window from becoming unresponsive
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}

	editState->f_updateMouse(window);
	editState->m_mousePos = SFML_utils::quantiseVec(editState->m_mousePos, Dim::TILE_SIZE_S.x);
	editState->m_mousePos.x = (int)(editState->m_mousePos.x + (Dim::TILE_SIZE_S.x * 0.5f));
	editState->m_mousePos.y = (int)(editState->m_mousePos.y + (Dim::TILE_SIZE_S.y * 0.5f));

	window.clear(Drawing::m_clearColor);
	Drawing::sfmlStart(window, true);
	editState->m_level->f_draw(dt, window);

	window.draw(editState->m_playerSprite);

	if (!editState->m_hideBorder)
		editState->m_border.f_draw(dt, window);
	editState->f_drawControls(window);

	if (editState->m_showConsole)
	{
		editState->m_console.f_take_input_and_draw(vec(), window);
	}
	Drawing::sfmlEnd(window);
	if (!window.hasFocus())
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		editState->m_playerSprite.setPosition(vecf(editState->m_mousePos));
	}

	if (SFML_utils::onMouseButtonReleased(sf::Mouse::Left))
		editState->m_level->f_setStartPos(vecf(editState->m_playerSprite.getPosition()));
}

void PlaceBlockState::f_run(float dt, sf::RenderWindow & window, EditState* editState)
{
	sf::Event event;//polled to stop window from becoming unresponsive
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0)
		{
			if (animationIndex < Drawing::m_animations.size() - 1)
				animationIndex++;
			else
				animationIndex = 0;
		}

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0)
		{
			if (animationIndex > 0)
				animationIndex--;
			else
				animationIndex = Drawing::m_animations.size() - 1;
		}
	}

	editState->f_updateMouse(window);
	vec rawMousePos = editState->m_mousePos;
	editState->m_mousePos = SFML_utils::quantiseVec(editState->m_mousePos, Dim::TILE_SIZE_S.x);
	

	Drawing::sfmlStart(window, true);
	editState->m_level->f_draw(dt, window);
	Drawing::m_animations[animationIndex].f_set_position(vecf(editState->m_mousePos) + (vecf(Dim::TILE_SIZE_S) * 0.5f));
	Drawing::m_animations[animationIndex].f_draw(window);
	Drawing::m_animations[animationIndex].f_update(dt);
	window.draw(editState->m_playerSprite);
	window.draw(editState->m_colliderCursor);
	if (!editState->m_hideBorder)
		editState->m_border.f_draw(dt, window);
	editState->f_drawControls(window);
	
	if (editState->m_showConsole)
	{
		editState->m_console.f_take_input_and_draw(vec(), window);
	}
	Drawing::sfmlEnd(window);
	if (!window.hasFocus())
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) //need to do before quanitsing due to collision detection of mouse position
	{
		editState->m_level->f_removeBlock(rawMousePos);
		editState->m_level->f_removeFlipBlock(rawMousePos);
	}	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if(animationIndex != 2)
			editState->m_level->f_addBlock(ptr<Block>(new Block(vecf(editState->m_mousePos) + vecf(Dim::TILE_SIZE_S / 2), Drawing::m_animations[animationIndex],editState->m_level)));
		else
			editState->m_level->f_addFlipBlock(ptr<FlipBlock>(new FlipBlock(vecf(editState->m_mousePos) + vecf(Dim::TILE_SIZE_S / 2), editState->m_level)));
	}

	
}

void PlaceFoodState::f_run(float dt, sf::RenderWindow & window, EditState* editState)
{
	sf::Event event;//polled to stop window from becoming unresponsive
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta > 0)
		{
			if (m_spriteIndex < Drawing::m_foodSprites.size() - 1)
				m_spriteIndex++;
			else
				m_spriteIndex = 0;
		}

		if (event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta < 0)
		{
			if (m_spriteIndex > 1)
				m_spriteIndex--;
			else
				m_spriteIndex = Drawing::m_foodSprites.size() - 1;
		}
	}

	editState->f_updateMouse(window);
	editState->m_mousePos = SFML_utils::quantiseVec(editState->m_mousePos, Dim::TILE_SIZE_S.x);



	Drawing::sfmlStart(window, true);
	editState->m_level->f_draw(dt, window);
	window.draw(editState->m_playerSprite);
	Drawing::m_foodSprites[m_spriteIndex]->setPosition(vecf(editState->m_mousePos));
	window.draw(*Drawing::m_foodSprites[m_spriteIndex]);
	if (!editState->m_hideBorder)
		editState->m_border.f_draw(dt, window);
	editState->f_drawControls(window);
	window.draw(editState->m_colliderCursor);

	if (editState->m_showConsole)
	{
		editState->m_console.f_take_input_and_draw(vec(), window);
	}
	Drawing::sfmlEnd(window);
	if (!window.hasFocus())
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		editState->m_level->f_addFoodItem(ptr<FoodItem>(new FoodItem(vecf(editState->m_mousePos), Drawing::m_foodSprites[m_spriteIndex].get(),m_spriteIndex)));
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		editState->m_level->f_removeFoodItem((vecf)editState->m_mousePos);
	}
}

void MoveLevelState::f_run(float dt, sf::RenderWindow& window,EditState* editState)
{
	sf::Event event;//polled to stop window from becoming unresponsive
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}

	editState->f_updateMouse(window);


	
	Drawing::sfmlStart(window, true);
	
	editState->m_level->f_draw(dt, window);
	Drawing::draw_grid(window);
	window.draw(editState->m_playerSprite);

	if (!editState->m_hideBorder)
		editState->m_border.f_draw(dt, window);
	editState->f_drawControls(window);
	editState->m_playerSprite.setPosition(editState->m_level->f_getStartPos());

	if (editState->m_showConsole)
	{
		editState->m_console.f_take_input_and_draw(vec(), window);
	}
	Drawing::sfmlEnd(window);
	if (editState->m_showConsole)
		return;

	if (!window.hasFocus())
		return;

	if (SFML_utils::onKeyPress(sf::Keyboard::D))
	{
		editState->m_level->f_moveLevel(Dir::RIGHT);
	}
	if (SFML_utils::onKeyPress(sf::Keyboard::A))
	{
		editState->m_level->f_moveLevel(Dir::LEFT);
	}
	if (SFML_utils::onKeyPress(sf::Keyboard::W))
	{
		editState->m_level->f_moveLevel(Dir::UP);
	}
	if (SFML_utils::onKeyPress(sf::Keyboard::S))
	{
		editState->m_level->f_moveLevel(Dir::DOWN);
	}
	
}

TilePalette::TilePalette(vecf pos, vec size):
	m_pos(pos),
	m_screenPosition(pos),
	m_size(size),
	m_isShowing(true),
	m_collider(pos,size,false)
{	
	m_collider.f_setActive(true);
	m_panel.setSize((vecf)size);
	m_panel.setFillColor(sf::Color::Blue);
	m_panel.setPosition(pos);

	
	int index = 0;
	vec palettePositions(0,0);
	for (auto & s : Drawing::m_tileset1)
	{
		m_tiles.push_back(Tile( (vecf)Dim::TILE_SIZE_S + m_pos +
			vecf(Dim::TILE_SIZE_S.x * palettePositions.x,Dim::TILE_SIZE_S.y * palettePositions.y),
			Dim::TILE_SIZE_S,
			s.get(),
			index));
		
		m_tiles.at(index).f_collider().f_setActive(true);

		palettePositions.x++;
		if (palettePositions.x == 4)
		{
			palettePositions.y++;
			palettePositions.x = 0; //TODO
		}
			
		index++;
	}
}

TilePalette::~TilePalette()
{}


void TilePalette::f_setPos(vecf pos)
{	
	m_pos = pos;

	m_collider.f_set_pos(m_pos);
	m_panel.setPosition(m_pos);

	vec palettePositions(0,0);
	for (auto & t : m_tiles)
	{
		
		t.f_setPos((vecf)Dim::TILE_SIZE_S + m_pos +
			vecf(Dim::TILE_SIZE_S.x * palettePositions.x, Dim::TILE_SIZE_S.y * palettePositions.y));
					

		palettePositions.x++;
		if (palettePositions.x == 4)
		{
			palettePositions.y++;
			palettePositions.x = 0; //TODO
		}	
	}

}
void TilePalette::f_input(vecf mousePos, sf::RenderWindow& window,EditState* editState)
{
	if (!m_isShowing)
		return;

	if (!m_collider.f_within_bounds(mousePos))
	{
		std::cout << "not within bounds\n";
		return;
	}
		
	else
		std::cout << "within bounds\n";

	for (auto&t : m_tiles)
	{
		if (t.f_collider().f_within_bounds(mousePos))
		{
			editState->m_currentSprite = t.f_getSprite();
			editState->m_spriteIndex = t.f_getSpriteIndex();
		}
	}

	
	

}

void TilePalette::f_draw(sf::RenderWindow& window)
{
	if (!m_isShowing)
		return;

	window.draw(m_panel);
	vecf pos;
	for (auto&t : m_tiles)
	{
		t.f_draw(0.0f, window);
	}
		
	m_collider.debug_draw(window);
}