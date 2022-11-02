#pragma once
#include "..\include\FlipBlock.h"
#include <iostream>
#include <sstream>
#include "..\include\Drawing.h"

FlipBlock::FlipBlock(vecf pos, Level* l)
	:
	Entity(pos, vec(),1000.0f),
	m_level(l),
	m_background(true)
	
{
	Entity::m_size = Dim::TILE_SIZE_S;
	Entity::m_collider.f_set_size(Dim::TILE_SIZE_S);

	for (int i = 0; i < Drawing::m_animations.size(); ++i)
	{
		if (Drawing::m_animations[i].f_identifier() == AnimationName::FLIPBLOCK)
			m_animation = Drawing::m_animations[i];
	}

	m_animation3dFront.f_setTextureArray(Drawing::m_spritesheetTexArray);
	m_animation3dFront.f_loadFrames(8, 1, 8);
	m_animation3dFront.f_set_frametime(0.01f);

	Entity::m_collider.f_setActive(true);
	Entity::m_collider.f_setActiveForPlayer(true);

	m_state.reset(new  FlipBlockForegroundState(this));
}
FlipBlock::FlipBlock(const FlipBlock & f)
	:
	Entity(f.m_pos, vec(), 1000.0f),
	m_level(f.m_level),
	m_background(f.m_background)
{
	Entity::m_pos = f.m_pos;

	Entity::m_size = Dim::TILE_SIZE_S;
	Entity::m_collider.f_set_size(Dim::TILE_SIZE_S);
	
	
	m_animation = f.m_animation;

	m_animation3dFront = f.m_animation3dFront;
	m_animation3dRight = f.m_animation3dRight;
	m_animation3dTop = f.m_animation3dTop;

	Entity::m_collider.f_setActive(true);
	Entity::m_collider.f_setActiveForPlayer(true);


	m_state.reset(new FlipBlockForegroundState(this));
}
FlipBlock::~FlipBlock()
{}

void FlipBlock::f_update(float dt, sf::RenderWindow & window)
{
	if (!m_animation3dFront.f_is_playing() && m_animation3dFront.f_currentFrame() == m_animation3dFront.f_totalframes() - 1)
	{

	}
}

void FlipBlock::f_activate(Dir direction)
{
	if (direction == Dir::UP)
	{
		m_state.reset(new FlipBlockBackgroundState(this));//states simply alter this classes members for correct state.
	}
	else if (direction == Dir::DOWN)
	{
		m_state.reset(new FlipBlockForegroundState(this));
	}
	else
	{
		std::cout << "Error direction for flip block.  Must be up or down\n";
		return;
	}
}

void FlipBlock::f_draw(float dt, sf::RenderWindow& window)
{
	m_animation.f_set_position(Entity::m_pos);
	m_animation.f_update(dt);
	m_animation.f_draw(window);
	//m_collider.debug_draw(window);

}

void FlipBlock::f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider /*= false*/)
{
	m_animation3dFront.f_set_position(vecf(m_pos.x, (-m_pos.y) - (Dim::TILE_SIZE_S.y * 0.5f)), Dim::TILE_SIZE_S.y * 0.5f);
	m_animation3dFront.f_update(dt);
	m_animation3dFront.f_draw(shader);

	m_animation3dTop.f_set_position(vecf(m_pos.x, (-m_pos.y) - (Dim::TILE_SIZE_S.y * 0.5f)));
	m_animation3dTop.f_update(dt);
	m_animation3dTop.f_draw(shader);

	m_animation3dRight.f_set_position(vecf(m_pos.x, (-m_pos.y) - (Dim::TILE_SIZE_S.y * 0.5f)));
	m_animation3dRight.f_update(dt);
	m_animation3dRight.f_draw(shader);
}

std::string FlipBlock::toString()
{
	std::stringstream ss;

	ss << "FlipBlock{position:" << SFML_utils::vecfToString(Entity::m_pos);
	ss << ",animation:" << static_cast<int>(m_animation.f_identifier());
	ss << "$}\n";

	//TODO
	return ss.str();
}

FlipBlockForegroundState::FlipBlockForegroundState(FlipBlock* parent)
{
	if (parent->m_background == true)
	{
		parent->m_background = false;
		parent->m_animation3dFront.f_reverse_frames();
		parent->m_animation3dFront.f_reset(true);
		parent->m_collider.f_setActiveForPlayer(true);//state will be opposite to whether active for player
	}
}

FlipBlockBackgroundState::FlipBlockBackgroundState(FlipBlock* parent)
{
	if (parent->m_background == false)
	{
		parent->m_background = true;
		parent->m_animation3dFront.f_reverse_frames();
		parent->m_animation3dFront.f_reset(true);
		parent->m_collider.f_setActiveForPlayer(false);//state will be opposite to whether active for player
	}
}