#pragma once
#include "Entity.h"
#include "Animation.h"
#include "AnimationGL.h"
#include <memory>
class Level;

class BlockState;

class Block : public Entity
{
public:

	Block(vecf pos, Animation animation,Level* l);
	Block(const Block & b);
	virtual ~Block();

	virtual void	f_update(float dt, sf::RenderWindow & window) override;
	virtual void	f_draw(float dt, sf::RenderWindow & window) override;
	virtual void	f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider = false) override;
	virtual void	f_setLevel(Level* l) { m_level = l; }
	virtual Level*	f_getLevel() { return m_level; }

	void	f_move(vecf newPos, Dir direction);
	void	f_move(vecf newPos, vec direction);
	bool	f_stillMoving();

	std::string toString();

private:

	BlockState* f_generateMoveState(Dir direction);	
	std::shared_ptr<BlockState>			m_state;
protected:

	Level*								m_level;
	Animation							m_animation;
	AnimationGL							m_animation3dFront;
	AnimationGL							m_animation3dRight;
	AnimationGL							m_animation3dTop;
};

class BlockState
{
public:

	virtual void	f_update(float dt, Block* b) = 0;

	bool	m_complete = false;
	vecf	m_targetPos;
	float	m_speed;
};

class BlockMoveLeftState : public BlockState
{
public:
	BlockMoveLeftState();
	void f_update(float dt, Block* b) override;
};

class BlockMoveRightState : public BlockState
{
public:
	BlockMoveRightState();
	void f_update(float dt, Block* b) override;

};

class BlockMoveUpState : public BlockState
{
public:
	BlockMoveUpState();
	void f_update(float dt, Block* b) override;
};

class BlockMoveDownState : public BlockState
{
public:
	BlockMoveDownState();
	void f_update(float dt, Block* b) override;
};

