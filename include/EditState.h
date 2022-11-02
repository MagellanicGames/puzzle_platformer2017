#pragma once
#include "PlayState.h"
#include "Level.h"
#include "Border.h"
#include "Console.h"
#include "EditorSubStates.h"

class EditState : public PlayState
{
public:
	friend class EditTileState;
	friend class EditColliderState;
	friend class PlaceStartState;
	friend class PlaceBlockState;
	friend class PlaceFoodState;
	friend class MoveLevelState;
	friend class TilePalette;

	EditState(vec levelDimensions = vec());
	~EditState();

	virtual void f_run(float dt, sf::RenderWindow& window) override;

	void f_updateMouse(sf::RenderWindow & window);
	void f_saveLevelToFile(std::string fileLocation) const;

	void f_setLevel(Level* l) override;

	Console* f_getConsole() { return &m_console; }

protected:

	void f_drawControls(sf::RenderWindow& window);
	void f_addTileDrag(vecf pos);
	void f_moveDragTiles();
	void f_drawTileDrag(float dt, sf::RenderWindow& window);
	

	float									m_camSpeed;
	sf::View								m_view;
	vecf									m_viewCenter;
	EditSubState*							m_subState;
	EditTileState							m_editTileState;
	EditColliderState						m_editColliderState;
	PlaceStartState							m_placeStartState;
	PlaceBlockState							m_placeBlockState;
	PlaceFoodState							m_placeFoodState;
	MoveLevelState							m_moveLevelState;
	
	std::vector<std::vector<ptr<Tile>>>		m_dragTiles;
	vec										m_mousePos;
	bool									m_hideBorder;
	bool									m_showConsole;
	unsigned int							m_spriteIndex;
	Sprite*									m_currentSprite;
	sf::RectangleShape						m_playerSprite;
	sf::RectangleShape						m_colliderCursor;
	Console									m_console;
	std::string								m_instructions = "(F1) Play (F2) Console (T)ile (P)alette Toggle (M)ove Level (C)ollider (Lmb) Place (Rmb) Remove (Mmb) Sample (B) Add Block (F)ood (S)tart Position";
	
};