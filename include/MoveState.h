#pragma once
#include "Common.h"

class Player;

class Block;

class MoveState
{
public:

	MoveState() {};
	virtual ~MoveState() {};
	virtual void move(float dt, Player* p) = 0;

	vecf	targetPos;
	float	speed;
	bool	complete = false;
	Dir		dir;
};

class MoveRightState : public MoveState
{
public:

	MoveRightState(Player* p);	

	void move(float dt, Player* p) override;
};

class MoveLeftState : public MoveState
{
public:
	MoveLeftState(Player* p);
	void move(float dt, Player* p) override;
};

class MoveRightDownState : public MoveState
{
public:
	MoveRightDownState(Player * p);
	void move(float dt, Player* p) override;
};

class MoveLeftDownState : public MoveState
{
public:
	MoveLeftDownState(Player * p);
	void move(float dt, Player* p);
};

class MoveRightUpState : public MoveState
{
public:
	MoveRightUpState(Player* p);
	void move(float dt, Player* p) override;
};

class MoveLeftUpState : public MoveState
{
public:
	MoveLeftUpState(Player* p);
	void move(float dt, Player* p) override;
};

class TurnLeftState : public MoveState
{
public:
	TurnLeftState(Player* p);
	void move(float dt, Player* p) override;
};

class TurnRightState : public MoveState
{
public:
	TurnRightState(Player* p);
	void move(float dt, Player* p) override;
};

class RtnBlockUpState : public MoveState
{
public:
	RtnBlockUpState(Player* p);
	void move(float dt, Player* p)override;
};

class RtnBlockDownState : public MoveState
{
public:
	RtnBlockDownState(Player* p);
	void move(float dt, Player* p)override;
};

class RtnBlockLeftState : public MoveState
{
public:
	RtnBlockLeftState(Player* p);
	void move(float dt, Player* p)override;
};

class RtnBlockRightState : public MoveState
{
public:
	RtnBlockRightState(Player* p);
	void move(float dt, Player* p)override;
};


class OnBlockMoveState : public MoveState
{
public:

	OnBlockMoveState(Block* b, Dir d);
	Block* block;
	void move(float dt, Player* p) override;
};