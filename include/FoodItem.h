#pragma once
#include "Entity.h"
#include "SpriteGL.h"

class FoodItem : public Entity
{
public:

	FoodItem(vecf pos, Sprite* sprite,int spriteIndex);
	~FoodItem();

	void	f_update(float dt, sf::RenderWindow & window) override;
	void	f_draw(float dt, sf::RenderWindow & window) override;
	void	f_draw3d(float dt, const ptr<Shader> & shader, bool drawCollider = false) override;

	std::string toString();

private:

	SpriteGL		m_sprite3d;
	Sprite*			m_sprite;
	int				m_spriteIndex;
};