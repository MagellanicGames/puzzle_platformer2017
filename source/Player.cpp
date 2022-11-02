#include "..\include\Player.h"
#include "..\include\Drawing.h"
#include"..\include\Level.h"
#include "..\include\GameState.h"
#include "..\include\MoveState.h"
#include <iostream>

Player::Player(vecf pos)
	:
	Entity(pos),
	m_debug(true),
	m_isDead(false)
{
	Entity::m_size = Dim::TILE_SIZE_S;
	Entity::m_collider.f_set_size(Entity::m_size);	
	Entity::m_speed = 200.0f;

	m_debugTargetPos.setFillColor(sf::Color::Green);
	m_debugTargetPos.setSize(vecf(4, 4));
	m_debugTargetPos.setOrigin(vecf(2, 2));

	m_3dAnimations[IDLERIGHT] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[IDLERIGHT].f_loadFrames(0, 0, 8);

	m_3dAnimations[IDLELEFT] = m_3dAnimations[IDLERIGHT];
	m_3dAnimations[IDLELEFT].f_flipFrames(true);

	m_3dAnimations[TURNLEFT] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[TURNLEFT].f_loadFrames(0, 5, 4);
	m_3dAnimations[TURNLEFT].f_start_animation();

	m_3dAnimations[TURNRIGHT] = m_3dAnimations[TURNLEFT];
	m_3dAnimations[TURNRIGHT].f_reverse_frames();


	m_3dAnimations[WALKRIGHT] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[WALKRIGHT].f_loadFrames(0, 1, 12);

	m_3dAnimations[WALKLEFT] = m_3dAnimations[WALKRIGHT];
	m_3dAnimations[WALKLEFT].f_flipFrames(true);

	m_3dAnimations[BLOCKUP] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[BLOCKUP].f_loadFrames(0, 2,4);

	m_3dAnimations[RTN_BLOCKUP] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[RTN_BLOCKUP].f_loadFrames(4, 2, 2);

	m_3dAnimations[BLOCKDOWN] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[BLOCKDOWN].f_loadFrames(0, 3, 4);

	m_3dAnimations[RTN_BLOCKDOWN] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[RTN_BLOCKDOWN].f_loadFrames(4, 3, 2);

	m_3dAnimations[BLOCKRIGHT] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[BLOCKRIGHT].f_loadFrames(0, 4, 4);

	m_3dAnimations[RTN_BLOCKRIGHT] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[RTN_BLOCKRIGHT].f_loadFrames(4, 4, 2);

	m_3dAnimations[BLOCKLEFT] = m_3dAnimations[BLOCKRIGHT];
	m_3dAnimations[BLOCKLEFT].f_flipFrames(true);

	m_3dAnimations[RTN_BLOCKLEFT] = m_3dAnimations[RTN_BLOCKRIGHT];
	m_3dAnimations[RTN_BLOCKLEFT].f_flipFrames(true);

	m_3dAnimations[FALLING] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[FALLING].f_loadFrames(0, 7, 4);

	m_3dAnimations[DEAD] = AnimationGL(Drawing::m_characterTexArray);
	m_3dAnimations[DEAD].f_loadFrames(0, 6, 7);
	

	for (auto & iter : m_3dAnimations)
	{
		iter.second.f_start_animation();
		iter.second.f_set_scale((vecf)Dim::TILE_SIZE_S * 2.0f);
		iter.second.f_set_frametime(0.1f);
	}


	m_3dAnimations[TURNLEFT].f_set_frametime(0.03f);
	m_3dAnimations[TURNRIGHT].f_set_frametime(0.03f);
	m_3dAnimations[WALKRIGHT].f_set_frametime(0.03f);	
	m_3dAnimations[WALKLEFT].f_set_frametime(0.03f);
	m_3dAnimations[DEAD].f_set_current_frame(0.05f);
	

	m_3dAnimations[WALKRIGHT].f_set_loop(true);
	m_3dAnimations[WALKLEFT].f_set_loop(true);
	m_3dAnimations[IDLERIGHT].f_set_loop(true);
	m_3dAnimations[IDLELEFT].f_set_loop(true);
	m_3dAnimations[FALLING].f_set_loop(true);

	m_3dAnimations[BLOCKRIGHT].f_set_frametime(0.025f);
	m_3dAnimations[BLOCKLEFT].f_set_frametime(0.025f);
	m_3dAnimations[BLOCKUP].f_set_frametime(0.025f);
	m_3dAnimations[BLOCKDOWN].f_set_frametime(0.025f);

	m_currentAnimation = &m_3dAnimations[IDLERIGHT];
	m_lastAnimation = m_currentAnimation;
	
}

Player::~Player()
{}

void Player::f_update(float dt, sf::RenderWindow& window)
{
	if (m_isDead)
	{
		f_playerDead(dt, window);
		return;
	}
	
	if(m_level->f_complete() == false)
		f_processControls(dt, window);		

	if (m_moveState != nullptr)
	{
		m_moveState->move(dt, this);
		if (m_moveState->complete)
		{
			if (!sf::Keyboard::isKeyPressed(Controls::LEFT) && m_moveState->dir == Dir::LEFT) //Stops animation stuttering as it switches between constant keypress
			{
				m_currentAnimation = &m_3dAnimations[IDLELEFT];
			}
			if (!sf::Keyboard::isKeyPressed(Controls::RIGHT) && m_moveState->dir == Dir::RIGHT) //Stops animation stuttering as it switches between constant keypress
			{
				m_currentAnimation = &m_3dAnimations[IDLERIGHT];
			}
			if (m_lastAnimation != nullptr)
				m_currentAnimation = m_lastAnimation;

			m_moveState.reset();
		}
	}

		
	if ( (m_level->f_withinBoundsOfBlock(Entity::m_pos, nullptr) || m_level->f_isCollisionTile(Entity::m_pos) &
		m_level->f_willTileCollidePlayer((vec)Entity::m_pos)))
	{
		m_isDead = true;
		std::cout << "You dieded\n";
	}

	m_level->f_removeFoodItem(Entity::m_pos);
	
	m_currentAnimation->f_set_position(Entity::m_pos);
	m_collider.f_set_pos(Entity::m_pos + Entity::m_collider_pos_correction);
}

void Player::f_processControls(float dt, sf::RenderWindow & window)
{
	if (sf::Keyboard::isKeyPressed(Controls::RIGHT) && !sf::Keyboard::isKeyPressed(Controls::BLOCK_MOVE)
		&& !sf::Keyboard::isKeyPressed(Controls::LEFT) && m_moveState == nullptr && m_level->f_areBlocksMoving() == false) //====================move right		
	{
		if (m_currentAnimation == &m_3dAnimations[IDLELEFT])
		{
			m_moveState.reset(new TurnRightState(this));
			return;
		}
		if (f_canMoveRight())		
			m_moveState.reset(new MoveRightState(this));
		else if (f_canMoveRightDown())		
			m_moveState.reset(new MoveRightDownState(this));
		else if (f_canMoveRightUp())		
			m_moveState.reset(new MoveRightUpState(this));	
		else
			m_currentAnimation = &m_3dAnimations[IDLERIGHT];
	}

	if (sf::Keyboard::isKeyPressed(Controls::LEFT) && !sf::Keyboard::isKeyPressed(Controls::BLOCK_MOVE)
		&& !sf::Keyboard::isKeyPressed(Controls::RIGHT) && m_moveState == nullptr && m_level->f_areBlocksMoving() == false) //=========================move left		
	{
		if (m_currentAnimation == &m_3dAnimations[IDLERIGHT])
		{
			m_moveState.reset(new TurnLeftState(this));
			return;
		}
		if (f_canMoveLeft())		
			m_moveState.reset(new MoveLeftState(this));					
		else if (f_canMoveLeftDown())
			m_moveState.reset(new MoveLeftDownState(this));	
		else if (f_canMoveLeftUp())
			m_moveState.reset(new MoveLeftUpState(this));		
		else
			m_currentAnimation = &m_3dAnimations[IDLELEFT];
	}


	if (sf::Keyboard::isKeyPressed(Controls::BLOCK_MOVE) && m_level->f_areBlocksMoving() == false && m_moveState == nullptr)//move blocks================================
	{
		if (SFML_utils::onKeyPress(Controls::RIGHT))
		{
			f_blockMove(Dir::RIGHT);
			if (m_currentAnimation != &m_3dAnimations[BLOCKRIGHT])
				f_setAnimation(BLOCKRIGHT,BLOCKRIGHT,true,true);			
			
		}
		if (SFML_utils::onKeyPress(Controls::LEFT))
		{
			f_blockMove(Dir::LEFT);
			if (m_currentAnimation != &m_3dAnimations[BLOCKLEFT])
				f_setAnimation(BLOCKLEFT, BLOCKLEFT, true, true);				
			
		}
		if (SFML_utils::onKeyPress(Controls::UP))
		{
			f_blockMove(Dir::UP);
			if (m_currentAnimation != &m_3dAnimations[BLOCKUP])
			{
				f_setAnimation(BLOCKUP, BLOCKUP, true, true);
			}
								

		}
		if (SFML_utils::onKeyPress(Controls::DOWN))
		{
			f_blockMove(Dir::DOWN);
			if (m_currentAnimation != &m_3dAnimations[BLOCKDOWN])
				f_setAnimation(BLOCKDOWN, BLOCKDOWN, true, true);
		}
	}


	if (SFML_utils::onKeyReleased(Controls::BLOCK_MOVE) && m_lastAnimation != nullptr) //once finished moving the block, return to previous animation
	{	
		if (m_lastAnimation == &m_3dAnimations[BLOCKUP])
			m_moveState.reset(new RtnBlockUpState(this));
		else if (m_lastAnimation == &m_3dAnimations[BLOCKDOWN])
			m_moveState.reset(new RtnBlockDownState(this));
		else if (m_lastAnimation == &m_3dAnimations[BLOCKLEFT])
			m_moveState.reset(new RtnBlockLeftState(this));
		else if(m_lastAnimation == &m_3dAnimations[BLOCKRIGHT])
			m_moveState.reset(new RtnBlockRightState(this));
	}

}

void Player::f_blockMove(Dir d)
{
	m_level->f_moveBlocks(d);
	if (m_moveState != nullptr)
		return;
	Block* b = m_level->f_getBlock(m_pos + vecf(0, Dim::TILE_SIZE_S.y));
	if (b != nullptr)
	{
		Dir direction;
		if (d == Dir::LEFT || d == Dir::RIGHT)
		{
			direction = Dir::HORIZONTAL;
		}			
		else if (d == Dir::UP || d == Dir::DOWN)
		{
			direction = Dir::VERTICAL;
		}			
		else
		{
			std::cout << "Error direction for player to move on block.\n";
			return;
		}

		m_moveState.reset(new OnBlockMoveState(b,direction));
	}
}

void Player::f_draw(float dt, sf::RenderWindow& window)
{
	m_currentAnimation->f_update(dt);
	//m_currentAnimation->f_draw(window);

	if (m_debug)
	{
		if (m_moveState != nullptr)
		{
			m_debugTargetPos.setPosition((vecf)m_moveState->targetPos);
		}
		window.draw(m_debugTargetPos);
		m_collider.debug_draw(window);
	}	
}


void Player::f_draw3d(float dt, const ptr<Shader> & shader,bool drawCollider /*= false*/)
{
	m_currentAnimation->f_set_position(vecf(m_pos.x, -m_pos.y));
	m_currentAnimation->f_update(dt);
	m_currentAnimation->f_draw(shader);
}

void Player::f_playerDead(float dt, sf::RenderWindow& window)
{
	if (!m_level->f_isCollisionTile(Entity::m_pos + vecf(0, Dim::TILE_SIZE_S.y / 2)))
	{
		Entity::m_pos.y += m_speed * 3 * dt;
		m_currentAnimation = &m_3dAnimations[FALLING];
	}
	else
	{
		m_currentAnimation = &m_3dAnimations[DEAD];
	}

	m_currentAnimation->f_set_position(Entity::m_pos);
	m_collider.f_set_pos(Entity::m_pos + Entity::m_collider_pos_correction);
}

bool Player::f_dyingAnimationFinished()
{
	if (m_currentAnimation == &m_3dAnimations[DEAD])
	{
		if (!m_currentAnimation->f_is_playing())
			return true;
		else
			return false;
	}
	else
		return false;
}
void Player::f_reset()
{
	m_isDead = false;
	m_currentAnimation = &m_3dAnimations[IDLERIGHT];
	m_moveState.reset();
	m_lastAnimation = nullptr;
	m_3dAnimations[FALLING].f_reset(true);
	m_3dAnimations[DEAD].f_reset(true);
}

/*
resetting refers to the new values set to the previous and current animations, not the newly set values
i.e. Resetting of m_lastAnimation is done after it has been set.
*/
void Player::f_setAnimation(AnimEnum ani,AnimEnum previousAnimation/*=DONTSET*/, bool resetCurrentAnimation/*= false*/,bool resetPreviousAnimation/*=false*/)
{
	if (m_currentAnimation != &m_3dAnimations[ani])//if the animation is the same, do nothing.
	{
		if (previousAnimation == DONTSET) //set to last used currentAnimation
			m_lastAnimation = m_currentAnimation;
		else//set to a specific animation
			m_lastAnimation = &m_3dAnimations[previousAnimation];
		
		if (resetPreviousAnimation  && m_lastAnimation != nullptr)
			m_lastAnimation->f_reset(true);

		m_currentAnimation = &m_3dAnimations[ani];//set currentanimation		
		if (resetCurrentAnimation)
			m_currentAnimation->f_reset(true);
	}
		
}

//================================================================================================Code that returns whether the player can move


bool Player::f_canMoveRight()
{
	bool right = m_level->f_isCollisionTile(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0))
		|| m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0.0f), nullptr);
	bool rightDown = m_level->f_isCollisionTile(Entity::m_pos + (vecf)Dim::TILE_SIZE_S)
		|| m_level->f_withinBoundsOfBlock(Entity::m_pos + (vecf)Dim::TILE_SIZE_S, nullptr);


	if (!right && rightDown)
		return true;
	else
		return false;
}

bool Player::f_canMoveRightDown()
{
	bool right = m_level->f_isCollisionTile(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0), nullptr);
	bool rightDown = m_level->f_isCollisionTile(Entity::m_pos + (vecf)Dim::TILE_SIZE_S) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos + (vecf)Dim::TILE_SIZE_S, nullptr);
	bool rightDown2 = m_level->f_isCollisionTile(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y * 2)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y * 2), nullptr);

	if (!right && !rightDown && rightDown2)
		return true;
	else
		return false;

}

bool Player::f_canMoveRightUp()
{
	bool right = m_level->f_isCollisionTile(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0)) || m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0), nullptr);
	bool rightUp = m_level->f_isCollisionTile(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, -Dim::TILE_SIZE_S.y)) || m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, -Dim::TILE_SIZE_S.y), nullptr);

	if (right && !rightUp)
		return true;
	else
		return false;
}

bool Player::f_canMoveLeft()
{
	bool left = m_level->f_isCollisionTile(Entity::m_pos - vecf(Dim::TILE_SIZE_S.x, 0)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos - vecf(Dim::TILE_SIZE_S.x, 0), nullptr);
	bool leftDown = m_level->f_isCollisionTile(Entity::m_pos + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y), nullptr);


	if ((!left && leftDown))
		return true;
	else
		return false;
}

bool Player::f_canMoveLeftDown()
{
	bool left = m_level->f_isCollisionTile(Entity::m_pos - vecf(Dim::TILE_SIZE_S.x, 0)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos - vecf(Dim::TILE_SIZE_S.x, 0), nullptr);
	bool leftDown = m_level->f_isCollisionTile(Entity::m_pos + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y), nullptr);
	bool leftDown2 = m_level->f_isCollisionTile(Entity::m_pos + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y * 2)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos + vecf(-Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y * 2), nullptr);

	if (!left && !leftDown && leftDown2)
		return true;
	else
		return false;
}

bool Player::f_canMoveLeftUp()
{
	bool left = m_level->f_isCollisionTile(Entity::m_pos - vecf(Dim::TILE_SIZE_S.x, 0)) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos - vecf(Dim::TILE_SIZE_S.x, 0), nullptr);
	bool leftUp = m_level->f_isCollisionTile(Entity::m_pos - (vecf)Dim::TILE_SIZE_S) ||
		m_level->f_withinBoundsOfBlock(Entity::m_pos - (vecf)Dim::TILE_SIZE_S, nullptr);

	if (left && !leftUp)
		return true;
	else
		return false;
}