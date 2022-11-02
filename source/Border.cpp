#include "..\include\Border.h"
#include "..\include\Drawing.h"

Border::Border(vec size, vecf position)
	:
	Entity(position),
	m_filled(false)
{
	Entity::m_size = size;
	m_background.setPosition(position + (vecf)Dim::TILE_SIZE_S);
	m_background.setSize(vecf(size - (Dim::TILE_SIZE_S * 2)));
	m_background.setFillColor(Drawing::m_clearColor);
}

Border::~Border()
{}

void Border::f_update(float dt, sf::RenderWindow & window)
{

}

void Border::f_draw(float dt, sf::RenderWindow & window) //size are exact, if border is 20 pixels wide, there are no extra pixels
{

	if (m_filled)
		window.draw(m_background);

	Drawing::m_BorderT.setPosition(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, 0));
	Drawing::m_BorderT.setScale(Entity::m_size.x - (Dim::TILE_SIZE_S.x * 2), Dim::SPRITE_SCALE.y);
	window.draw(Drawing::m_BorderT);	
	
	Drawing::m_BorderB.setPosition(Entity::m_pos + vecf(Dim::TILE_SIZE_S.x, Entity::m_size.y - Dim::TILE_SIZE_S.y));
	Drawing::m_BorderB.setScale(Entity::m_size.x - (Dim::TILE_SIZE_S.x * 2), Dim::SPRITE_SCALE.y);
	window.draw(Drawing::m_BorderB);

	Drawing::m_BorderL.setPosition(Entity::m_pos + vecf(0, Dim::TILE_SIZE_S.y));
	Drawing::m_BorderL.setScale(Dim::SPRITE_SCALE.x, Entity::m_size.y - (Dim::TILE_SIZE_S.y * 2));
	window.draw(Drawing::m_BorderL);

	Drawing::m_BorderR.setPosition(Entity::m_pos + vecf(Entity::m_size.x - Dim::TILE_SIZE_S.x, Dim::TILE_SIZE_S.y));
	Drawing::m_BorderR.setScale(Dim::SPRITE_SCALE.x, Entity::m_size.y - (Dim::TILE_SIZE_S.y * 2));
	window.draw(Drawing::m_BorderR);
	
	Drawing::m_BorderTL.setPosition(Entity::m_pos);
	window.draw(Drawing::m_BorderTL);
	Drawing::m_BorderTR.setPosition(Entity::m_pos + vecf(Entity::m_size.x - Dim::TILE_SIZE_S.x,0));
	window.draw(Drawing::m_BorderTR);
	Drawing::m_BorderBR.setPosition(Entity::m_pos + vecf(Entity::m_size - Dim::TILE_SIZE_S));
	window.draw(Drawing::m_BorderBR);
	Drawing::m_BorderBL.setPosition(Entity::m_pos + vecf(0,Entity::m_size.y - Dim::TILE_SIZE_S.y));
	window.draw(Drawing::m_BorderBL);
}