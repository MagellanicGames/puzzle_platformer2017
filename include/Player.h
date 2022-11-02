#pragma once
#include "Entity.h"
#include "Animation.h"
#include "AnimationGL.h"
#include <memory>
#include <map>

class Level;
class MoveState;

enum AnimEnum{DONTSET,IDLELEFT,IDLERIGHT,
	TURNRIGHT,WALKRIGHT,
	WALKLEFT,TURNLEFT,
	BLOCKUP,RTN_BLOCKUP,
	BLOCKDOWN,RTN_BLOCKDOWN,
	BLOCKLEFT,RTN_BLOCKLEFT,
	BLOCKRIGHT,RTN_BLOCKRIGHT,
	FALLING,DEAD};

class Player : public Entity
{
public:
	friend class MoveState;
	friend class DrawDelegate3d;
	friend class PlayState3d;

	Player(vecf pos);
	~Player();

	void	f_update(float dt, sf::RenderWindow & window) override;
	void	f_draw(float dt, sf::RenderWindow & window) override;
	void	f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider = false) override;

	void	f_setLevel(Level* l) { m_level = l; }
	Level*	f_getLevel() { return m_level; }

	bool	f_dyingAnimationFinished();

	void	f_setAnimation(AnimEnum ani,AnimEnum previousAnimation = DONTSET,bool resetCurrentAnimation = false,bool resetPreviousAnimation = false);
	AnimationGL*	f_getCurrentAnimation() { return m_currentAnimation; }

	void	f_reset();

	bool f_isDead()const { return m_isDead; }


private:

	void f_processControls(float dt, sf::RenderWindow & window);

	bool f_canMoveLeft();
	bool f_canMoveRight();
	bool f_canMoveRightDown();
	bool f_canMoveRightUp();
	bool f_canMoveLeftDown();
	bool f_canMoveLeftUp();

	void f_blockMove(Dir d);

	void f_playerDead(float dt, sf::RenderWindow& window);

	bool								m_isDead;

	bool								m_debug;

	std::map<AnimEnum, AnimationGL>	m_3dAnimations;

	AnimationGL*						m_currentAnimation;
	AnimationGL*						m_lastAnimation;

	Level*								m_level;

	sf::RectangleShape					m_debugTargetPos;

	std::shared_ptr<MoveState>			m_moveState;
	std::shared_ptr<MoveState>			m_returnMoveState;
};