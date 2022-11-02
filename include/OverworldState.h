#pragma once
#include "GameState.h"
#include "OrthoCamera.h"
#include "OverworldDialog.h"
#include "OverworldPlayer.h"

class Overworld;
class SpriteGL;

class OverworldState : public GameState
{
public:

	OverworldState();
	~OverworldState();

	virtual void f_run(float dt, sf::RenderWindow& window) override;

	virtual void f_update(float dt, sf::RenderWindow & window) override;
	virtual void f_drawGL(float dt, ptr<Shader> & shader)override;
	virtual void f_drawSFML(float dt, sf::RenderWindow & window) override;
	virtual void f_takeInput(float dt, sf::RenderWindow & window) override;

	void f_notifyOfLevelCompleted();

	std::string f_currentLevelName()const { return m_currentLevelName; }


protected:

	void f_updatePlayerPosition(Dir direction);

	ptr<Overworld>	m_overworld;
	OverworldPlayer	m_player;
	std::string		m_currentLevelName;
	OrthoCamera		m_camera;
	OverworldDialog m_overworldDialog;
	bool			m_inMenu;
	

};