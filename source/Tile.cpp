#include "..\include\Tile.h"
#include <sstream>
#include "..\include\SFML_utils.h"
#include <iostream>
#include "..\include\Drawing.h"
#include <glm\gtc\type_ptr.hpp>

std::vector<unsigned int> Tile::m_backgroundTileIndices = { 15,16,17,33,34,35,51,52,53 };

Tile::Tile(vecf pos, vec size, Sprite* sprite, unsigned int spriteIndex)
	:
	Entity(pos),
	m_sprite(sprite),
	m_spriteIndex(spriteIndex),
	m_isBackgroundTile(false)
{
	Entity::m_size = size;
	Entity::m_collider.f_set_size(size);
	Entity::m_collider_pos_correction = (vecf)Dim::TILE_SIZE_S / 2.0f;
	Entity::m_collider.f_set_pos(Entity::m_pos + Entity::m_collider_pos_correction);
	Entity::m_type = Type::TILE;
	

	for (auto & i : m_backgroundTileIndices)
	{
		if (spriteIndex == i)
			m_isBackgroundTile = true;
	}

	if (m_spriteIndex < 18)
		m_spriteCube.f_setCubeMap(Drawing::m_cubeset1[m_spriteIndex]);
	else
		m_spriteCube.f_setCubeMap(Drawing::m_cubeset1[17]);
}

	

Tile::~Tile()
{
}

void Tile::f_update(float dt, sf::RenderWindow& window)
{

}

void Tile::f_draw(float dt, sf::RenderWindow& window)
{

	m_sprite->setPosition(sf::Vector2f((int)m_pos.x, (int)m_pos.y));
	window.draw(*m_sprite);

}

void Tile::f_draw3d(float dt, const ptr<Shader> & shader,bool drawColliders /*= false*/)
{
	glUniform1i(shader->uniform("drawColliders"), drawColliders);
	static glm::vec3 yellow(1, 1, 0);
	static glm::vec3 green(0, 1, 0);

	if(Entity::m_collider.f_isActiveForPlayer())
		glUniform3fv(shader->uniform("colliderColor"), 1, glm::value_ptr(yellow));
	else if(!Entity::m_collider.f_isActiveForPlayer())
		glUniform3fv(shader->uniform("colliderColor"), 1, glm::value_ptr(green));
	
	float z = m_isBackgroundTile ? -Dim::TILE_SIZE_S.y : 0.0f;
	m_spriteCube.f_setPos(vecf(m_pos.x, -m_pos.y), z);
	m_spriteCube.f_draw(dt, shader);
	
	
}

std::string Tile::toString()
{
	std::stringstream ss;

	ss << "Tile{position:" << SFML_utils::vecfToString(m_pos) << ",spriteIndex:" << std::to_string(m_spriteIndex);
	ss << ",colliderActive:";
	if (m_collider.f_isActive())
		ss << "true";
	else
		ss << "false";
	ss <<"}$\n";
	return ss.str();
}