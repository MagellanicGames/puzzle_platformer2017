#pragma once
#include "Common.h"
#include "Collider.h"

class Shader;
class Shape;

enum Type{FOOD, TILE};

class Entity
{
public:
	Entity(vecf start_pos = vecf(), vec direction = vec(),float speed = 2.0f);
	virtual ~Entity();

	virtual void f_setPos(vecf & v)
	{
		m_pos.x = v.x;
		m_pos.y = v.y;
		m_collider.f_set_pos(m_pos + m_collider_pos_correction);
	}

	virtual void	f_update(float dt, sf::RenderWindow & window);
	virtual void	f_draw(float dt, sf::RenderWindow & window);
	virtual void	f_draw3d(float dt, const ptr<Shader> & shader,bool drawCollider = false);

	int				f_id() { return m_idnum; }
	vecf&			f_pos() { return m_pos; }
	vec&			f_size() { return m_size; }
	vec&			f_dir() { return m_dir; }
	Collider&		f_collider() { return m_collider; }
	void			f_set_collider_pos_correction(vecf v) { m_collider_pos_correction = v; }	

	float f_speed()const { return m_speed; }

	Type			m_type;

protected:

	virtual bool	f_canMove(vec position_to_move) const;
		
	Collider		m_collider;
	vecf			m_collider_pos_correction;
	vecf			m_pos;
	vec				m_dir;
	vec				m_size;
	float			m_speed;
	float			m_zpos;
	int				m_idnum;
};

