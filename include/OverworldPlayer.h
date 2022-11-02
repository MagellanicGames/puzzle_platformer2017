#pragma once
#include "Common.h"
#include <glm\vec3.hpp>

class SpriteGL;
class Shader;

class OverworldPlayer {
public:

	OverworldPlayer(glm::vec3 pos = glm::vec3());
	~OverworldPlayer();

	void update(float dt);
	void draw(ptr<Shader>& shader);

	void setPos(glm::vec3 v);
	void setTargetPos(glm::vec3 v) { m_targetPos = v; }

	glm::vec3 pos();

	SpriteGL* getSprite() { return m_playerSprite.get(); }

	bool isMoving()const { return m_moving; }

private:

	void updateIfMoving(float dt);

	ptr<SpriteGL>	m_playerSprite;
	glm::vec3		m_targetPos;
	bool			m_moving;
};