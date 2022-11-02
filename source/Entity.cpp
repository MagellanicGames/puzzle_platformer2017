#include "..\include\Entity.h"
#include <sstream>
#include "..\include\Drawing.h"

#include "..\include\Shape.h"
#include "..\include\Primitives.h"
#include "..\include\Shader.h"

Entity::Entity(vecf start_pos, vec direction,float speed)
	:
	m_pos(start_pos),
	m_dir(direction),
	m_speed(speed),
	m_collider(Collider(start_pos,m_size)),
	m_collider_pos_correction(vec(0,0))
	
{
	static int id_num_counter = 0;
	m_idnum = id_num_counter;
	id_num_counter++;	

}


Entity::~Entity()
{
}

void Entity::f_update(const float dt, sf::RenderWindow & window)
{
	m_collider.f_set_pos(m_pos + m_collider_pos_correction);
}

void Entity::f_draw(const float dt, sf::RenderWindow & window)
{
	
}

void Entity::f_draw3d(float dt, const ptr<Shader> & shader,bool drawCollider/*=false*/)
{

}

bool Entity::f_canMove(vec position_to_move) const
{
	return true;
}