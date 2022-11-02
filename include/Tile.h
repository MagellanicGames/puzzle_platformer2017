#pragma once
#include "Entity.h"
#include "SpriteCube.h"

class Tile : public Entity
{
public:

	friend class DrawDelegate;
	friend class DrawDelegate3d;

	Tile(vecf pos, vec size, Sprite* sprite, unsigned int spriteIndex);
	~Tile();

	void f_update(float dt, sf::RenderWindow& window) override;
	void f_draw(float dt, sf::RenderWindow& window) override;
	void f_draw3d(float dt, const ptr<Shader> & shader,bool drawColliders = false) override;

	void f_setSprite(Sprite* const s) { m_sprite = s; }
	Sprite* f_getSprite() { return m_sprite; }

	unsigned int f_getSpriteIndex() const { return m_spriteIndex; }
	std::string toString();

	static std::vector<unsigned int> m_backgroundTileIndices;

	bool f_isBackground()const  { return m_isBackgroundTile; }

private:

	SpriteCube		m_spriteCube;
	Sprite*			m_sprite;
	unsigned int	m_spriteIndex;
	bool			m_isBackgroundTile;
};