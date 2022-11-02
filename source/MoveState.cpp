#include "..\include\MoveState.h"
#include "..\include\Player.h"
#include "..\include\Block.h"


MoveRightState::MoveRightState(Player * p)
{
	dir = RIGHT;
	targetPos = p->f_collider().f_pos() + vecf(Dim::TILE_SIZE_S.x, 0);
	speed = p->f_speed();

	p->f_setAnimation(WALKRIGHT,IDLERIGHT,false,true);

}
void MoveRightState::move(float dt, Player* p)
{	
	if (p->f_pos().x < targetPos.x)
	{
		vecf newPos = p->f_pos() + vecf(speed * dt, 0);
		if (newPos.x > targetPos.x)
			p->f_setPos(targetPos);
		else
			p->f_setPos(newPos);
	}		
	else
		complete = true;
}

MoveLeftState::MoveLeftState(Player* p)
{
	dir = LEFT;
	targetPos = p->f_collider().f_pos() + vecf(-Dim::TILE_SIZE_S.x, 0);
	speed = p->f_speed();

	p->f_setAnimation(WALKLEFT,IDLELEFT,false,true);
}

void MoveLeftState::move(float dt, Player* p)
{
	vecf newPos = p->f_pos() + vecf(-(speed  * dt),0);
	if (p->f_pos().x > targetPos.x)
	{
		if (newPos.x < targetPos.x)
			newPos.x = targetPos.x;
		p->f_setPos(newPos);
	}
	else
		complete = true;
}

MoveRightDownState::MoveRightDownState(Player * p)
{
	dir = RIGHT;
	targetPos = p->f_collider().f_pos() + vecf(Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y);
	speed = p->f_speed();

	p->f_setAnimation(WALKRIGHT, IDLERIGHT, false, true);
}

void MoveRightDownState::move(float dt, Player*p)
{
	vecf newPos((p->f_pos().x + ((speed)* dt)), p->f_pos().y + ((speed)* dt));
	if (p->f_pos().x < targetPos.x)
	{
		if (newPos.x > targetPos.x)
			newPos.x = targetPos.x;
		if (newPos.y > targetPos.y)
			newPos.y = targetPos.y;
		p->f_setPos(newPos);
	}		
	else
		complete = true;
}

MoveRightUpState::MoveRightUpState(Player * p)
{
	dir = RIGHT;
	targetPos = p->f_collider().f_pos() + vecf(Dim::TILE_SIZE_S.x, -Dim::TILE_SIZE_S.y);
	speed = p->f_speed();

	p->f_setAnimation(WALKRIGHT, IDLERIGHT, false, true);
}

void MoveRightUpState::move(float dt, Player* p)
{
	vecf newPos(p->f_pos().x + (speed * dt), p->f_pos().y - ((speed)* dt));
	if (p->f_pos().x < targetPos.x)
	{
		if (newPos.x > targetPos.x)
			newPos.x = targetPos.x;
		if (newPos.y < targetPos.y)
			newPos.y = targetPos.y;
		p->f_setPos(newPos);		
	}

	else
		complete = true;
}

MoveLeftDownState::MoveLeftDownState(Player* p)
{
	dir = LEFT;
	targetPos = p->f_collider().f_pos() + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y);
	speed = p->f_speed();

	p->f_setAnimation(WALKLEFT, IDLELEFT, false, true);
}

void MoveLeftDownState::move(float dt, Player* p)
{
	vecf newPos(p->f_pos().x - (speed * dt), p->f_pos().y + (speed  * dt));
	if (p->f_pos().x > targetPos.x || p->f_pos().y < targetPos.y)
	{
		if (newPos.x < targetPos.x)
			newPos.x = targetPos.x;
		if (newPos.y > targetPos.y)
			newPos.y = targetPos.y;
		p->f_setPos(newPos);
	}
		
	else
		complete = true;
}

MoveLeftUpState::MoveLeftUpState(Player* p)
{
	dir = LEFT;
	targetPos = p->f_collider().f_pos() + vecf(-Dim::TILE_SIZE_S);
	speed = p->f_speed();

	p->f_setAnimation(WALKLEFT, IDLELEFT, false, true);
}

void MoveLeftUpState::move(float dt, Player* p)
{
	vecf newPos((p->f_pos().x - (speed * dt)), p->f_pos().y - (speed * dt));
	if (p->f_pos().x > targetPos.x)
	{
		if (newPos.x < targetPos.x)
			newPos.x = targetPos.x;
		if (newPos.y < targetPos.y)
			newPos.y = targetPos.y;
		p->f_setPos(newPos);
	}
		
	else
		complete = true;
}

TurnLeftState::TurnLeftState(Player* p)
{
	p->f_setAnimation(AnimEnum::TURNLEFT,IDLELEFT,true,true);
	dir = LEFT;
}

void TurnLeftState::move(float dt, Player* p)
{
	if (p->f_getCurrentAnimation()->f_is_playing() == false)//has finished animation
		complete = true;
}


TurnRightState::TurnRightState(Player* p)
{
	p->f_setAnimation(AnimEnum::TURNRIGHT, IDLERIGHT, true,true);
	dir = RIGHT;
}

void TurnRightState::move(float dt, Player* p)
{
	if (p->f_getCurrentAnimation()->f_is_playing() == false)//has finished animation
		complete = true;
}

RtnBlockUpState::RtnBlockUpState(Player* p)
{
	p->f_setAnimation(AnimEnum::RTN_BLOCKUP, IDLERIGHT, true,true);
}


void RtnBlockUpState::move(float dt, Player* p)
{
	if (p->f_getCurrentAnimation()->f_is_playing() == false)//has finished animation
		complete = true;
}

RtnBlockDownState::RtnBlockDownState(Player* p)
{
	p->f_setAnimation(AnimEnum::RTN_BLOCKDOWN, IDLERIGHT, true, true);
}


void RtnBlockDownState::move(float dt, Player* p)
{
	if (p->f_getCurrentAnimation()->f_is_playing() == false)//has finished animation
		complete = true;
}

RtnBlockLeftState::RtnBlockLeftState(Player* p)
{
	p->f_setAnimation(AnimEnum::RTN_BLOCKLEFT, IDLELEFT, true, true);
}


void RtnBlockLeftState::move(float dt, Player* p)
{
	if (p->f_getCurrentAnimation()->f_is_playing() == false)//has finished animation
		complete = true;
}

RtnBlockRightState::RtnBlockRightState(Player* p)
{
	p->f_setAnimation(AnimEnum::RTN_BLOCKRIGHT, IDLERIGHT, true, true);
}


void RtnBlockRightState::move(float dt, Player* p)
{
	if (p->f_getCurrentAnimation()->f_is_playing() == false)//has finished animation
		complete = true;
}

OnBlockMoveState::OnBlockMoveState(Block* b, Dir d)
	:
	block(b)
{
	dir = d;
}

void OnBlockMoveState::move(float dt, Player* p)
{
	if (dir == Dir::HORIZONTAL)
	{
		if (block->f_stillMoving())
			p->f_setPos(vecf(block->f_pos().x, p->f_pos().y));
		else
			complete = true;
	}
	else
	{
		if (block->f_stillMoving())
			p->f_setPos(vecf(p->f_pos().x,block->f_pos().y - Dim::TILE_SIZE_S.y));
		else
			complete = true;
	}	
}