#include "..\include\Level.h"
#include "..\include\Drawing.h"
#include "..\include\Block.h"
#include "..\include\FlipBlock.h"
#include "..\include\Collider.h"
#include <sstream>
#include <iostream>

Level::Level()
	:
	m_playerStartPos(vec(Dim::RES_X * 0.5,Dim::RES_Y * 0.5)),
	m_drawColliders(false),
	m_endLevel(false),
	m_statsScreenTimer(5.0f),
	m_par(10),
	m_score(3000)
{
	m_levelDimensions = vec(m_tiles[0].size(), m_tiles.size());

	for (auto & a : m_tiles)
		for (auto & t : a)
			t.reset();		


	for (auto & a : m_collisionTiles)
		for (auto & t : a)
			t.reset();

	for (auto & a : m_foodItems)
		for (auto & f : a)
			f.reset();

	//ptr<FlipBlock> fb(new FlipBlock(m_playerStartPos + vecf(Dim::TILE_SIZE_S.x,0.0f), this));//TODO:Remove
	//m_flipBlocks.push_back(fb);
}			

Level::Level(const Level& l)
	:
	m_endLevel(false),
	m_statsScreenTimer(5.0f),
	m_par(l.m_par),
	m_score(l.m_score)
{
	m_levelDimensions = l.m_levelDimensions;
	m_playerStartPos = l.m_playerStartPos;
	m_drawColliders = l.m_drawColliders;
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 30; x++)
		{
			Tile* tile = l.m_tiles[y][x].get();
			Tile* collision = l.m_collisionTiles[y][x].get();
			FoodItem* food = l.m_foodItems[y][x].get();
			if (tile != nullptr)
			{
				ptr<Tile>newTile(new Tile(*tile));
				f_addTile(newTile);
			}				
			if (collision != nullptr)
			{
				ptr<Tile>newCollisionTile(new Tile(*collision));
				f_addCollisionTile(newCollisionTile);
			}
				
			if (food != nullptr)
			{
				ptr<FoodItem>newFood(new FoodItem(*food));
				f_addFoodItem(newFood);
			}
				
		}
	}

	for (auto & b : l.m_blocks)
	{
		m_blocks.push_back(ptr<Block>(new Block(*b)));
		m_blocks.at(m_blocks.size() - 1)->f_setLevel(this);
	}

	for (auto & fb : l.m_flipBlocks)
	{
		m_flipBlocks.push_back(ptr<FlipBlock>(new FlipBlock(*fb)));
		m_flipBlocks.at(m_flipBlocks.size() - 1)->f_setLevel(this);
	}

	if (m_tilesVec.size() < 1)
		f_setDrawColliders(true);
}

Level::~Level()
{
	std::cout << "Level deleted\n";
}

void Level::f_update(float dt, sf::RenderWindow& window)
{
	if (m_complete)
	{
		f_endOfLevel(dt, window);
		return;
	}
	for (auto & b : m_blocks)	
		b->f_update(dt, window);

	for (auto& f : m_foodItemsVec)
		f->f_update(dt, window);

	if (m_foodItemsVec.size() < 1)
		m_complete = true;

	m_numMovesMade = m_movesMade.size() / m_blocks.size();
	m_score = ScoreKeeper::startingScore - (m_numMovesMade * ScoreKeeper::pointsPerMove);
	m_bonusScore = m_numMovesMade <= m_par ? (((m_par - m_numMovesMade) + 1) * ScoreKeeper::pointsPerMoveUnderPar) : 0;
	ScoreKeeper::par = m_par;
	
}

void Level::f_endOfLevel(float dt, sf::RenderWindow & window)
{
	ScoreKeeper::movesTotal = m_numMovesMade;
	ScoreKeeper::scoreTotal = m_score;
	ScoreKeeper::bonusPoints = m_bonusScore;

	if (m_statsScreenTimer > 0.0f)
	{
		m_statsScreenTimer -= dt;
		return;
	}

	if (SFML_utils::onKeyPress(Controls::ACCEPT))
		m_endLevel = true;

	if (m_endLevel)
	{
		EventQueue::f_addEvent(Event::LEVELCOMPLETED);
		
	}
		
}

bool Level::f_canEndLevel()const
{
	if (m_statsScreenTimer < 0.0f)
		return true;
	else
		return false;
}

void Level::f_moveBlocks(Dir d)
{
	vec direction_vector;

	switch (d)
	{
		case Dir::UP:direction_vector.y = -1; break;
		case Dir::DOWN:direction_vector.y = 1; break;
		case Dir::LEFT:direction_vector.x = -1; break;
		case Dir::RIGHT:direction_vector.x = 1; break;
	}

	f_calculateNewPositions(direction_vector);

	if (d == Dir::UP || d == Dir::DOWN)
	{
		for (auto & fb : m_flipBlocks)
		{
			fb->f_activate(d);
		}
	}
}

void Level::f_draw(float dt, sf::RenderWindow& window,bool drawColliders)
{
	

	for (auto&t : m_tilesVec)
		t->f_draw(dt, window);	

	for (auto & b : m_blocks)
		b->f_draw(dt, window);

	for (auto& fb : m_flipBlocks)
		fb->f_draw(dt, window);

	for (auto & f : m_foodItemsVec)
		f->f_draw(dt, window);
		
	if (!m_drawColliders && !drawColliders)
		return;

	for(auto & t : m_collisionTilesVec)
		t->f_collider().debug_draw(window);	
				
}

void Level::f_draw3d(float dt, const ptr<Shader> & shader)
{
	if (m_drawColliders)
	{
		for (auto & t : m_collisionTilesVec)
			t->f_draw3d(dt, shader, true);
	}
	else
	{
		for (auto & t : m_tilesVec)
			t->f_draw3d(dt, shader);
	}


	for (auto & b : m_blocks)
		b->f_draw3d(dt, shader);

	for (auto & fb : m_flipBlocks)
		fb->f_draw3d(dt, shader);

	for (auto & f : m_foodItemsVec)
		f->f_draw3d(dt, shader);	



}

void Level::f_addTile(ptr<Tile>& t)
{
	if (!f_withinLevelBounds(t->f_pos()))
	{
		t = nullptr;
		return;
	}
		
	
	sf::Vector2i arrayPos(t->f_pos().x / Dim::TILE_SIZE_S.x, t->f_pos().y / Dim::TILE_SIZE_S.y);
	Tile* oldTile = m_tiles[arrayPos.y][arrayPos.x].get();
	if (oldTile != nullptr)
	{
		f_removeEntity(oldTile);
		m_tiles[arrayPos.y][arrayPos.x].reset();		
	}
		
	
	m_tiles[arrayPos.y][arrayPos.x] = t;	
	m_tilesVec.push_back(t.get()); //add to entity vector

}

void Level::f_addCollisionTile(ptr<Tile>& t,bool collideWithPlayer)
{

	if (!f_withinLevelBounds(t->f_pos()))
	{
		t = nullptr;
		return;
	}
	sf::Vector2i arrayPos(t->f_pos().x / Dim::TILE_SIZE_S.x, t->f_pos().y / Dim::TILE_SIZE_S.y);

	Tile* oldTile = m_collisionTiles[arrayPos.y][arrayPos.x].get();
	if (oldTile != nullptr)
	{		
		f_removeEntity(oldTile);
		m_collisionTiles[arrayPos.y][arrayPos.x].reset();
	}
		

	
	t->f_collider().f_setActiveForPlayer(collideWithPlayer);
	m_collisionTiles[arrayPos.y][arrayPos.x] = t;
	m_collisionTilesVec.push_back(t.get());
}

void Level::f_deleteTile(vec mousePos)
{
	if (!f_withinLevelBounds((vecf)mousePos))
		return;

	sf::Vector2i arrayPos(mousePos.x / Dim::TILE_SIZE_S.x, mousePos.y / Dim::TILE_SIZE_S.y);
	Tile* t = m_tiles[arrayPos.y][arrayPos.x].get();
	if (t != nullptr)
	{
		f_removeEntity(t);
		m_tiles[arrayPos.y][arrayPos.x].reset();
	}

}

void Level::f_deleteCollisionTile(vec mousePos)
{
	if (!f_withinLevelBounds((vecf)mousePos))
		return;

	sf::Vector2i arrayPos(mousePos.x / Dim::TILE_SIZE_S.x, mousePos.y / Dim::TILE_SIZE_S.y);
	if (m_collisionTiles[arrayPos.y][arrayPos.x] != nullptr)
	{		
		f_removeEntity(m_collisionTiles[arrayPos.y][arrayPos.x].get());
		m_collisionTiles[arrayPos.y][arrayPos.x].reset();
	}
}

Tile* Level::f_getTile(vec mousePos)
{
	if (!f_withinLevelBounds((vecf)mousePos))
		return nullptr;

	sf::Vector2i arrayPos(mousePos.x / Dim::TILE_SIZE_S.x, mousePos.y / Dim::TILE_SIZE_S.y);

	return m_tiles[arrayPos.y][arrayPos.x].get();
}

bool Level::f_isCollisionTile(vecf mousePos)
{
	if (!f_withinLevelBounds((vecf)mousePos))
		return false;

	sf::Vector2i arrayPos(mousePos.x / Dim::TILE_SIZE_S.x, mousePos.y / Dim::TILE_SIZE_S.y);


	if (m_collisionTiles[arrayPos.y][arrayPos.x] == nullptr)
	{
		for (auto & flipBlock : m_flipBlocks)
		{
			if (flipBlock->f_collider().f_within_bounds(mousePos))
				return true;
		}
		return false;
	}		
	else
	{
		return true;
	}
		
}

bool Level::f_willTileCollidePlayer(vec mousePos)
{
	if (!f_withinLevelBounds((vecf)mousePos))
		return false;

	sf::Vector2i arrayPos(mousePos.x / Dim::TILE_SIZE_S.x, mousePos.y / Dim::TILE_SIZE_S.y);
	Tile* tile = m_collisionTiles[arrayPos.y][arrayPos.x].get();

	if (tile == nullptr)
		return false;
	else if (tile->f_collider().f_isActiveForPlayer())
		return true;
	else
		return false;
}

vecf Level::f_findNextCollisionTile(vecf pos, vec dir)
{
	int maxTilesSearched = 20;

	
	int i = 0;
	while (i < maxTilesSearched)
	{
		vecf positionToCheck = pos + vecf(Dim::TILE_SIZE_S.x * i * dir.x, Dim::TILE_SIZE_S.y * i *  dir.y);
		if (f_isCollisionTile(positionToCheck) == false)
			i++;
		else
		{
			return pos + vecf((int)Dim::TILE_SIZE_S.x * i * dir.x, (int)Dim::TILE_SIZE_S.y * i * dir.y);
		}
	}

	return vecf();
	
}
void Level::f_clearTiles()
{
	for (auto & a : m_tiles)
	{
		for (auto & t : a)
		{
			if (t != nullptr)
			{
				t.reset();
			}
		}
	}

	m_tilesVec.clear();
}

void Level::f_addBlock(ptr<Block>& b)
{
	int i = 0;
	for (auto & block : m_blocks)
	{
		if (block->f_pos() == b->f_pos())
		{
			m_blocks.erase(m_blocks.begin() + i);
			break;
		}
		i++;
	}
	m_blocks.push_back(b);
}

void Level::f_removeBlock(vec mousePos)
{
	int i = 0;
	for (auto& block : m_blocks)
	{
		if (block->f_collider().f_within_bounds(vecf(mousePos)))					
			break;		
		i++;
	}

	if(i > -1 && i < m_blocks.size())
		m_blocks.erase(m_blocks.begin() + i);			
	
}

void Level::f_addFlipBlock(ptr<FlipBlock>& fb)
{
	int i = 0;
	for (auto & block : m_flipBlocks)
	{
		if (block->f_pos() == fb->f_pos())
		{
			m_flipBlocks.erase(m_flipBlocks.begin() + i);
			break;
		}
		i++;
	}
	m_flipBlocks.push_back(fb);
}

void Level::f_removeFlipBlock(vec mousePos)
{
	int i = 0;
	for (auto& block : m_flipBlocks)
	{
		if (block->f_collider().f_within_bounds(vecf(mousePos)))
			break;
		i++;
	}

	if (i > -1 && i < m_flipBlocks.size())
		m_flipBlocks.erase(m_flipBlocks.begin() + i);
}

bool Level::f_withinBoundsOfBlock(vecf position, Entity* exclude)
{
	for (auto & b : m_blocks)
	{
		if(exclude != nullptr)
			if (b.get() == exclude)
				continue;
		if (b->f_collider().f_within_bounds(position))
			return true;
	}

	for (auto & b : m_flipBlocks)
	{
		if (exclude != nullptr)
			if (b.get() == exclude)
				continue;
		if (b->f_collider().f_within_bounds(position) && b->f_collider().f_isActiveForPlayer())
			return true;
	}
	return false;
}

void Level::f_calculateNewPositions(vec direction) // returns vector of blocks that have same final position
{

	for (auto & b : m_blocks) //calculate new positions, add code for flipblocks		
	{
		PosBlock* pb = new PosBlock(f_findNextCollisionTile(b->f_pos(), direction), b.get());
		m_newBlockPositions[pb->pos.y / Dim::TILE_SIZE_S.y][pb->pos.x / Dim::TILE_SIZE_S.x].push_back(pb); //map to level grid
	}

	//blocks need to be stacked opposite to direction of movement, hence multiplying by -1
	vecf positionOffset(Dim::TILE_SIZE_S.x * direction.x * -1, Dim::TILE_SIZE_S.y * direction.y * -1);

	for (auto& ar : m_newBlockPositions) //iterate through each row/column of blocks
	{
		for (auto & vector : ar)
		{
			if (vector.size() < 1)//if there is only one block, skip
				continue;

			if (direction.x == 1)//sort blocks in the vector by position in the desired direction
			{
				std::sort(vector.begin(), vector.end(), [](PosBlock* a, PosBlock* b)// Right
				{
					return a->block->f_pos().x > b->block->f_pos().x;
				});
			}
			else if (direction.x == -1)
			{
				std::sort(vector.begin(), vector.end(), [](PosBlock* a, PosBlock* b)// Left
				{
					return a->block->f_pos().x < b->block->f_pos().x;
				});
			}
			else if (direction.y == -1)
			{
				std::sort(vector.begin(), vector.end(), [](PosBlock* a, PosBlock* b)// Up
				{
					return a->block->f_pos().y < b->block->f_pos().y;
				});
			}
			else if (direction.y == 1)
			{
				std::sort(vector.begin(), vector.end(), [](PosBlock* a, PosBlock* b)//  Down
				{
					return a->block->f_pos().y > b->block->f_pos().y;
				});
			}
			else
			{
				std::cout << "Cannot move blocks, error direction.\n";
			}

			

			for (int i = 0; i < vector.size(); i++) //now calculate the new positions (the stacking)
			{
				vecf oldPosition(vector[i]->pos);
				vecf newPosition(oldPosition.x + (positionOffset.x * i), oldPosition.y + (positionOffset.y * i));
				vector[i]->pos = newPosition;//correct new position based on other calculated positions	
				
			}
			
			for (auto& pb : vector) // move the blocks now that the new position has been calculated
				pb->block->f_move(pb->pos + positionOffset, direction);			

			vector.clear(); //clear the vector for next run			
		}
	}

}

bool Level::f_containsPosBlock(PosBlock* p, std::vector<PosBlock*> & vector)
{
	for (auto & pos : vector)
	{
		if (pos == p)
			return true;
	}
	return false;
}

bool Level::f_areBlocksMoving()
{
	for (auto & b : m_blocks)
		if (b->f_stillMoving())
			return true;

	return false;
}

Block* Level::f_getBlock(vecf pos)
{
	for (auto & b : m_blocks)
	{
		if (b->f_collider().f_within_bounds(pos))
			return b.get();
	}
	return nullptr;
}

void Level::f_addFoodItem(ptr<FoodItem> item)
{
	if (!f_withinLevelBounds(item->f_pos()))
	{
		item = nullptr;
		return;
	}
	sf::Vector2i arrayPos(item->f_pos().x / Dim::TILE_SIZE_S.x, item->f_pos().y / Dim::TILE_SIZE_S.y);

	if (m_foodItems[arrayPos.y][arrayPos.x] != nullptr)
	{
		f_removeEntity(m_foodItems[arrayPos.y][arrayPos.x].get());				
	}	
	m_foodItems[arrayPos.y][arrayPos.x] = item;	
	m_foodItemsVec.push_back(item.get());
}

void Level::f_removeFoodItem(vecf pos)
{
	if(!f_withinLevelBounds(pos))
		return;

	sf::Vector2i arrayPos(pos.x / Dim::TILE_SIZE_S.x, pos.y / Dim::TILE_SIZE_S.y);
	if (m_foodItems[arrayPos.y][arrayPos.x] != nullptr)
	{
		f_removeEntity(m_foodItems[arrayPos.y][arrayPos.x].get());
		m_foodItems[arrayPos.y][arrayPos.x].reset();
	}
		
}

void Level::f_moveLevel(Dir d)
{
	if (d == Dir::HORIZONTAL || d == Dir::VERTICAL)
		return;
	vecf direction(0, 0);

	switch (d)
	{
		case Dir::DOWN:direction.y = Dim::TILE_SIZE_S.y; break;
		case Dir::UP:direction.y = -Dim::TILE_SIZE_S.y; break;
		case Dir::LEFT:direction.x = -Dim::TILE_SIZE_S.x; break;
		case Dir::RIGHT:direction.x = Dim::TILE_SIZE_S.x; break;
	}

	std::vector<Entity*> entities;

	entities.insert(entities.end(), m_tilesVec.begin(), m_tilesVec.end());
	entities.insert(entities.end(), m_collisionTilesVec.begin(), m_collisionTilesVec.end());
	entities.insert(entities.end(), m_foodItemsVec.begin(), m_foodItemsVec.end());

	for (auto& e : entities) // check no tiles/objects will be moved out of bounds
	{
		vecf newPos(e->f_pos() + direction);	

		if (newPos.x < 0.0f && d == Dir::LEFT)
			return;
		if (newPos.x > (m_levelDimensions.x - 1) * Dim::TILE_SIZE_S.x && d == Dir::RIGHT)
			return;
		if (newPos.y < 0.0f && d == Dir::UP)
			return;
		if(newPos.y > (m_levelDimensions.y - 1) * Dim::TILE_SIZE_S.y && d == Dir::DOWN)
			return;		
	}

	f_clearEntityVectors();//clear vectors as tiles will be readded when reinserted

	ptr<std::array<std::array<ptr<Tile>, 300>, 200>> copyTiles(new std::array<std::array<ptr<Tile>,300>,200>(m_tiles));
	ptr<std::array<std::array<ptr<Tile>, 300>, 200>> copyColliders(new std::array<std::array<ptr<Tile>, 300>, 200>(m_collisionTiles));
	ptr<std::array<std::array<ptr<FoodItem>, 300>, 200>> copyFood(new std::array<std::array<ptr<FoodItem>, 300>, 200>(m_foodItems));

	for (int i = 0; i < copyTiles->size(); i++)
	{
		m_tiles[i].fill(nullptr);
		m_collisionTiles[i].fill(nullptr);
		m_foodItems[i].fill(nullptr);
	}	

	for (int y = 0; y < m_levelDimensions.y; y++)
	{
		for (int x = 0; x < m_levelDimensions.x; x++)
		{
			ptr<Tile> t(copyTiles->at(y).at(x));
			if (t != nullptr)
			{
				t->f_setPos(t->f_pos() + direction);
				f_addTile(t);
			}

			ptr<Tile> c(copyColliders->at(y).at(x));
			if (c != nullptr)
			{
				c->f_setPos(c->f_pos() + direction);
				f_addCollisionTile(c);
			}

			ptr<FoodItem> f(copyFood->at(y).at(x));
			if (f != nullptr)
			{
				f->f_setPos(f->f_pos() + direction);
				f_addFoodItem(f);
			}
		}
	}	

	for (auto& b : m_blocks)
	{
		b->f_setPos(b->f_pos() + direction);
	}

	m_playerStartPos += direction;
	
}

void Level::f_removeEntity(Entity* entityToRemove)
{
	int i = 0;
	for (auto & e : m_foodItemsVec)
	{
		if (e == entityToRemove)
		{
			m_foodItemsVec.erase(m_foodItemsVec.begin() + i);
			return;
		}			
		i++;
	}

	i = 0;
	for (auto & e : m_collisionTilesVec)
	{
		if (e == entityToRemove)
		{
			m_collisionTilesVec.erase(m_collisionTilesVec.begin() + i);
			return;
		}
		i++;
	}


	i = 0;
	for (auto & e : m_tilesVec)
	{
		if (e == entityToRemove)
		{
			m_tilesVec.erase(m_tilesVec.begin() + i);
			return;
		}
		i++;
	}
}

bool Level::f_withinLevelBounds(vecf position)
{
	if (position.x < 0.0f || position.y < 0.0f
		|| position.x >(m_levelDimensions.x - 1) * Dim::TILE_SIZE_S.x || position.y >(m_levelDimensions.y - 1) * Dim::TILE_SIZE_S.y)	
		return false;
	else
		return true;
}

void Level::f_clearEntityVectors()
{
	m_tilesVec.clear();
	m_collisionTilesVec.clear();
	m_foodItemsVec.clear();
}
std::string Level::toString()
{
	std::stringstream ss;

	ss << "Level{\n";
	for (auto &a : m_tiles)
	{
		for (auto & t : a)
		{
			if (t != nullptr)
			{
				ss << t->toString();
			}
		}
	}
	ss << "$endOfTiles\n";
	ss << "startPos:" << SFML_utils::vecfToString(m_playerStartPos) << "$\n";
	ss << "CollisionTiles\n";
	for (auto &a : m_collisionTiles)
	{
		for (auto & t : a)
		{
			if (t != nullptr)
			{
				ss << t->toString();
			}
		}
	}

	ss << "$endOfCollisionTiles\n";

	for (auto& b : m_blocks)
	{
		ss << b->toString();
	}

	ss << "$endOfBlocks\n";
	
	for (auto &a : m_foodItems)
	{
		for (auto & f : a)
		{
			if (f != nullptr)
			{
				ss << f->toString();
			}
		}
	}
	
	ss << "$endOfFood\n";

	for (auto&fb : m_flipBlocks)
	{
		ss << fb->toString();
	}
	ss << "$endOfFlipBlocks\n";
	ss << "par{" << m_par << "}\n";
	ss << "$endOfFile}";
	return ss.str();
}