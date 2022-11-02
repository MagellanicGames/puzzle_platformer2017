#pragma once
#include "GameState.h"
#include "Border.h"
#include "Level.h"
#include "Player.h"
#include "OrthoCamera.h"
#include "ScoreDialog.h"
#include "IngameOptionsDialog.h"
#include "DeadDialog.h"


class PlayState3d : public GameState
{
public:
	friend class Player;
	PlayState3d();
	~PlayState3d();


	virtual void f_run(float dt, sf::RenderWindow& window) override;

	virtual void f_update(float dt, sf::RenderWindow& window) override;
	virtual void f_drawGL(float dt, ptr<Shader> & shader)override;
	virtual void f_drawSFML(float dt, sf::RenderWindow & window) override;
	virtual void f_takeInput(float dt, sf::RenderWindow & window) override;

	virtual void	f_setLevel(Level* l);
	Level*			f_getLevel() { return m_level; }

protected:

	Border		m_border;
	Level*		m_level;
	Player		m_player;
	OrthoCamera m_camera;
	ScoreDialog m_scoreDialog;
	DeadDialog	m_deadDialog;
	IngameOptionsDialog m_ingameOptionsDialog;

private:

	bool		m_gamePaused;
};