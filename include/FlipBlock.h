#pragma once
#include "Entity.h"
#include "Animation.h"
#include "AnimationGL.h"
#include <memory>

class Level;
class FlipBlockState;

class FlipBlock : public Entity
{
public:

	friend class FlipBlockForegroundState;
	friend class FlipBlockBackgroundState;

	FlipBlock(vecf pos, Level* l);
	FlipBlock(const FlipBlock & f);
	~FlipBlock();

	void f_activate(Dir direction);

	virtual void	f_update(float dt, sf::RenderWindow & window) override;
	virtual void	f_draw(float dt, sf::RenderWindow & window) override;
	virtual void	f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider = false) override;


	void f_setLevel(Level* l) { m_level = l; }
	std::string toString();

private:

	bool								m_background;//when up is pressed
	Level*								m_level;
	std::shared_ptr<FlipBlockState>		m_state;
	Animation							m_animation;
	AnimationGL							m_animation3dFront;
	AnimationGL							m_animation3dRight;
	AnimationGL							m_animation3dTop;
};

class FlipBlockState
{
public:

	FlipBlockState() {};
	virtual ~FlipBlockState() {};
};

class FlipBlockForegroundState: public FlipBlockState
{
public:
	FlipBlockForegroundState(FlipBlock* parent);
};

class FlipBlockBackgroundState : public FlipBlockState
{
public:
	FlipBlockBackgroundState(FlipBlock* parent);

};