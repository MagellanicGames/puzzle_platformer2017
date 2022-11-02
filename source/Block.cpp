#include "..\include\Block.h"
#include "..\include\Level.h"
#include "..\include\Drawing.h"
#include "..\include\EventQueue.h"
#include <iostream>
#include <sstream>

Block::Block(vecf pos, Animation a,Level* l)
	:
	Entity(pos,vec(),1000.0f),
	m_level(l),
	m_animation(a)
	
{
	Entity::m_size = Dim::TILE_SIZE_S;
	Entity::m_collider.f_set_size(Entity::m_size);
	Entity::m_collider.f_setActive(true);

	if (a.f_identifier() == AnimationName::GRNBLOCK1)	
		m_animation3dFront = AnimationGL(Drawing::m_spritesheetTexArray, m_animation, 8, 2);
		
	else	
		m_animation3dFront = AnimationGL(Drawing::m_spritesheetTexArray, m_animation, 8, 3);

	m_animation3dTop = AnimationGL(m_animation3dFront, Dir::TOP);
	m_animation3dRight = AnimationGL(m_animation3dFront, Dir::RIGHT);
	
}

Block::Block(const Block & b)
	:
	Entity(b.m_pos,vec(),1000.0f),
	m_level(b.m_level),
	m_animation(b.m_animation),
	m_animation3dFront(b.m_animation3dFront),
	m_animation3dRight(b.m_animation3dRight),
	m_animation3dTop(b.m_animation3dTop)
{
	Entity::m_size = b.m_size;
	Entity::m_collider.f_set_size(b.m_size);
	Entity::m_collider.f_setActive(true);
}

Block::~Block()
{}

void Block::f_update(float dt, sf::RenderWindow & window)
{
	if (m_state != nullptr)
	{
		if(!m_state->m_complete)
			m_state->f_update(dt, this);
		else
		{
			EventQueue::f_addEvent(Event::PLAYIMPACTSOUND);
			m_state.reset();
		}
	}		
}

void Block::f_move(vecf v, vec direction)
{
	if (direction.y == -1)
		f_move(v, Dir::UP);
	else if (direction.y == 1)
		f_move(v, Dir::DOWN);
	else if (direction.x == 1)
		f_move(v, Dir::RIGHT);
	else if (direction.x == -1)
		f_move(v, Dir::LEFT);
}

void Block::f_move(vecf v, Dir direction)
{
	if (m_state == nullptr)
	{
		m_state.reset(f_generateMoveState(direction));
		m_state->m_speed = Entity::m_speed;
		m_state->m_targetPos = v;
	}
}

BlockState* Block::f_generateMoveState(Dir direction)
{
	switch (direction)
	{
	case Dir::UP:return new BlockMoveUpState(); break;
	case Dir::DOWN:return new BlockMoveDownState(); break;
	case Dir::LEFT:return new BlockMoveLeftState(); break;
	case Dir::RIGHT:return new BlockMoveRightState(); break;
	}
	return nullptr;
}

void Block::f_draw(float dt, sf::RenderWindow& window)
{
	m_animation.f_set_position(Entity::m_pos);
	m_animation.f_update(dt);
	m_animation.f_draw(window);
	//m_collider.debug_draw(window);
	
}

void Block::f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider /*= false*/)
{
	m_animation3dFront.f_set_position(vecf(m_pos.x, (-m_pos.y) - (Dim::TILE_SIZE_S.y * 0.5f)),Dim::TILE_SIZE_S.y * 0.5f);
	m_animation3dFront.f_update(dt);
	m_animation3dFront.f_draw(shader);

	m_animation3dTop.f_set_position(vecf(m_pos.x, (-m_pos.y) - (Dim::TILE_SIZE_S.y * 0.5f)));
	m_animation3dTop.f_update(dt);
	m_animation3dTop.f_draw(shader);

	m_animation3dRight.f_set_position(vecf(m_pos.x, (-m_pos.y) - (Dim::TILE_SIZE_S.y * 0.5f)));
	m_animation3dRight.f_update(dt);
	m_animation3dRight.f_draw(shader);
}

bool Block::f_stillMoving()
{
	if (m_state == nullptr)
		return false;
	else
		return true;
}

std::string Block::toString()
{
	std::stringstream ss;

	ss << "Block{position:" << SFML_utils::vecfToString(Entity::m_pos);
	ss << ",animation:" << static_cast<int>(m_animation.f_identifier());
	ss << "$}\n";
	return ss.str();
}

//==================================================================Move States

BlockMoveRightState::BlockMoveRightState() 
{ 
	EventQueue::f_addEvent(Event::MOVERIGHT); 
}

void BlockMoveRightState::f_update(float dt, Block* b)
{
	
	if (b->f_pos().x < m_targetPos.x)
	{
		vecf newPos = b->f_pos() + vecf(m_speed * dt, 0.0f);		
		if (newPos.x > m_targetPos.x)
			b->f_setPos(m_targetPos);
		else
			b->f_setPos(newPos);
	}
	else
	{
		m_complete = true;
	}

}

BlockMoveLeftState::BlockMoveLeftState() 
{ 
	EventQueue::f_addEvent(Event::MOVELEFT);
}

void BlockMoveLeftState::f_update(float dt, Block* b)
{
	if (b->f_pos().x > m_targetPos.x)
	{
		vecf newPos = b->f_pos() - vecf(m_speed * dt, 0.0f);		
		if (newPos.x < m_targetPos.x)
			b->f_setPos(m_targetPos);
		else
			b->f_setPos(newPos);
	}
	else
	{
		m_complete = true;
	}
}

BlockMoveUpState::BlockMoveUpState() 
{
	EventQueue::f_addEvent(Event::MOVEUP);
}

void BlockMoveUpState::f_update(float dt, Block* b)
{
	if (b->f_pos().y > m_targetPos.y)
	{
		vecf newPos = b->f_pos() - vecf(0.0, m_speed * dt);		
		if (newPos.y < m_targetPos.y)
			b->f_setPos(m_targetPos);
		else
			b->f_setPos(newPos);
	}
	else
	{
		m_complete = true;
	}
}

BlockMoveDownState::BlockMoveDownState()
{ 
	EventQueue::f_addEvent(Event::MOVEDOWN);
}

void BlockMoveDownState::f_update(float dt, Block* b)
{
	if (b->f_pos().y < m_targetPos.y)
	{
		vecf newPos = b->f_pos() + vecf(0.0f, m_speed * dt);		
		if (newPos.y > m_targetPos.y)
			b->f_setPos(m_targetPos);
		else
			b->f_setPos(newPos);
	}
	else
	{
		m_complete = true;
	}
}
	
