#pragma once
#include "Common.h"
#include "Tile.h"
#include <array>
#include "FoodItem.h"
#include <memory>
#include "EventQueue.h"

class Block;
class FlipBlock;

struct PosBlock
{

	PosBlock(vecf p, Block* b)
		:
		pos(p),
		block(b) {}
	
	vecf		pos;
	Block*	block;
};

class Level
{
public:
	friend class EditState;
	Level();
	Level(const Level& l);
	~Level();

	void		f_update(float dt, sf::RenderWindow& window);
	void		f_draw(float dt, sf::RenderWindow& window,bool drawColliders = false);
	void		f_draw3d(float dt,const ptr<Shader> & shader);
	void		f_addTile(ptr<Tile> & t);
	
	void		f_deleteTile(vec mousePos);	
	Tile*		f_getTile(vec mousePos);
	bool		f_isCollisionTile(vecf MousePos);
	void		f_addCollisionTile(ptr<Tile> & t,bool collideWithPlayer = true);
	void		f_deleteCollisionTile(vec mousePos);
	void		f_addBlock(ptr<Block> & b);
	void		f_removeBlock(vec mousePos);
	Block*		f_getBlock(vecf v);

	void		f_addFlipBlock(ptr<FlipBlock> & fb);
	void		f_removeFlipBlock(vec mousePos);

	void		f_clearTiles();	

	void		f_addFoodItem(ptr<FoodItem> item);
	void		f_removeFoodItem(vecf pos);

	vecf		f_getStartPos() { return m_playerStartPos; }
	void		f_setStartPos(vecf p) { m_playerStartPos = p; }

	vecf		f_findNextCollisionTile(vecf pos, vec dir);//find collision tiles from given position in a direction
	bool		f_withinBoundsOfBlock(vecf position, Entity* exclude); //detect blocks

	void		f_moveBlocks(Dir dir);
	bool		f_areBlocksMoving();

	void		f_setDrawColliders(bool b) { m_drawColliders = b; }
	bool		f_getDrawColliders() { return m_drawColliders; }
	void		f_moveLevel(Dir dir);

	bool		f_withinLevelBounds(vecf pos);
	bool		f_willTileCollidePlayer(vec mousePos); //used for blocks that stop blocks but not the player
	bool		f_complete()const { return m_complete; }

	std::vector<Event>& f_returnMovesMade() { return m_movesMade; }
	int					f_numMovesMade() { return m_movesMade.size() / m_movesMade.size(); }
	void				f_recordMove(Event moveMade) { m_movesMade.push_back(moveMade); }
	int					f_numberOfBlocks() { return m_blocks.size(); }
	vec					f_levelDimensions()const { return m_levelDimensions; }
	std::string			toString();

	bool		f_canEndLevel()const;

	void f_setPar(int p) { m_par = p; }

private:	
	
	void f_calculateNewPositions(vec direction);
	bool f_containsPosBlock(PosBlock* p, std::vector<PosBlock*> & vector);
	void f_removeEntity(Entity* e);
	void f_clearEntityVectors();

	void f_endOfLevel(float dt,sf::RenderWindow & window);
	
	bool														m_complete;
	bool														m_endLevel;
	bool														m_drawColliders;

	float														m_statsScreenTimer;
	std::array<std::array<ptr<Tile>, 300>, 200>					m_tiles;
	std::vector<Entity*>										m_tilesVec;

	std::array<std::array<ptr<Tile>, 300>, 200>					m_collisionTiles;
	std::vector<Entity*>										m_collisionTilesVec;	

	std::array<std::array<ptr<FoodItem>, 300>, 200>				m_foodItems;
	std::vector<Entity*>										m_foodItemsVec;

	std::array < std::array<std::vector<PosBlock*>, 300>, 200>	m_newBlockPositions; //2d array of vectors
	std::vector<ptr<Block>>										m_blocks;

	std::vector<ptr<FlipBlock>>									m_flipBlocks;

	vecf														m_playerStartPos;

	vec															m_levelDimensions;

	std::vector<Event>											m_movesMade;

	int															m_numMovesMade;
	int															m_score;
	int															m_bonusScore;
	int															m_par;
};