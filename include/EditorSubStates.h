#pragma once
#include "Common.h"
#include "Collider.h"
#include "Tile.h"

class EditState;

class TilePalette
{
public:

			TilePalette(vecf pos, vec size);
			~TilePalette();

	void f_setPos(vecf pos);

	void	f_input(vecf mousePos, sf::RenderWindow& window, EditState* editState);
	void	f_draw(sf::RenderWindow& window);

	sf::RectangleShape	m_panel;
	bool				m_isShowing;
	vecf				m_pos;
	vecf				m_screenPosition;//position on screen with no transforms
	vec					m_size;
	Collider			m_collider;
	std::vector<Tile>	m_tiles;
};

class EditSubState
{
public:

	virtual void f_run(float dt, sf::RenderWindow & window, EditState* editState) = 0;
};

class EditTileState : public EditSubState
{
public:

	EditTileState();
	TilePalette m_tilepalette;

	void f_run(float dt, sf::RenderWindow & window, EditState* editState) override;	
	bool paletteActiveAndWithinBounds(vec mousePos);
};

class EditColliderState : public EditSubState
{
public:

	void f_run(float dt, sf::RenderWindow & window, EditState* editState) override;
};


class PlaceStartState : public EditSubState
{
public:

	void f_run(float dt, sf::RenderWindow & window, EditState* editState) override;
};

class PlaceBlockState : public EditSubState
{
public:
	void f_run(float dt, sf::RenderWindow & window, EditState* editState) override;

	int animationIndex = 0;
};

class PlaceFoodState : public EditSubState
{
public:
	void f_run(float dt, sf::RenderWindow & window, EditState* editState) override;

	int m_spriteIndex = 0;
};

class MoveLevelState : public EditSubState
{
public:
	void f_run(float dt, sf::RenderWindow & window, EditState* editState) override;
};