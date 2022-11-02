#pragma once
#include "..\include\FoodItem.h"
#include <sstream>
#include "..\include\Drawing.h"

FoodItem::FoodItem(vecf pos, Sprite* sprite,int spriteIndex)
	:
	Entity(pos),
	m_sprite(sprite),
	m_spriteIndex(spriteIndex),
	m_sprite3d(Drawing::m_spritesheetTexArray,spriteIndex + 8)
{
	Entity::m_size = (Dim::TILE_SIZE_S);
	Entity::m_type = Type::FOOD;
	m_collider.f_set_size(Entity::m_size);
	m_collider.f_set_pos(Entity::m_pos);
	m_collider.f_setActive(true);	
	m_sprite->setPosition(pos);
	m_sprite3d.f_setBackfaceCull(false);
}

FoodItem::~FoodItem()
{}

void FoodItem::f_update(float dt, sf::RenderWindow & window)
{
	m_sprite3d.f_set_rotation(0.0f, 100.0f * dt, 0.0f);
}

void FoodItem::f_draw(float dt, sf::RenderWindow & window)
{
	m_sprite->setPosition(Entity::m_pos);
	window.draw(*m_sprite);
}

void FoodItem::f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider /*= false*/)
{
	m_sprite3d.f_set_position(vecf(m_pos.x + (Dim::TILE_SIZE_S.x * 0.5f), (-m_pos.y) - Dim::TILE_SIZE_S.y));
	m_sprite3d.f_draw(shader);
}

std::string FoodItem::toString()
{
	std::stringstream ss;

	ss << "FoodItem{position:" << SFML_utils::vecfToString(m_pos) << ",spriteIndex:" << std::to_string(m_spriteIndex) << "}$\n";

	return ss.str();
}