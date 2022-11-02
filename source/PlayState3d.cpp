#include "..\include\PlayState3d.h"
#include "..\include\Drawing.h"

PlayState3d::PlayState3d()
	:
	m_border(vec(Dim::RES_X, Dim::RES_Y - Dim::TILE_SIZE_S.y), vecf()),
	m_player(vecf()),
	m_camera(glm::vec3(0, 0, 100), Dim::RES_X, Dim::RES_Y),
	m_gamePaused(false)
{	
	m_camera.set2D();
}

PlayState3d::~PlayState3d()
{}

void PlayState3d::f_run(float dt, sf::RenderWindow & window)
{
	GameState::f_run(dt, window);

	GameState::f_pollEvents(dt, window);
	if (!m_gamePaused)
		f_update(dt, window);		

	GameState::f_draw(dt, window);	

	f_takeInput(dt, window);
}

void PlayState3d::f_update(float dt, sf::RenderWindow& window)
{
	m_camera.update(dt);
	m_player.f_update(dt, window);
	m_level->f_update(dt, window);
}

void PlayState3d::f_drawGL(float dt, ptr<Shader> & shader)
{
	m_camera.focus(glm::vec3(m_player.f_pos().x, -m_player.f_pos().y, 0));
	m_camera.passParameters(Drawing::m_shader);
	m_level->f_draw3d(dt, Drawing::m_shader);
	m_player.f_draw3d(dt, Drawing::m_shader);
}

void PlayState3d::f_drawSFML(float dt, sf::RenderWindow & window)
{
	m_border.f_draw(dt, window);
	Text::draw(vecf(100, 100), 48, "Press 'c' to switch camera mode.", window);
	window.draw(m_fadeRect);
	if (m_level->f_complete())
	{
		//std::to_string(m_level->f_returnMovesMade().size() / m_level->f_numberOfBlocks())
		m_scoreDialog.f_setShowContinueText(m_level->f_canEndLevel());
		m_scoreDialog.f_draw(dt, window);
	}

	if (m_player.f_dyingAnimationFinished())
		m_deadDialog.f_draw(dt, window);

	if (m_gamePaused)
		m_ingameOptionsDialog.f_draw(dt, window);
}

void PlayState3d::f_takeInput(float dt, sf::RenderWindow & window)
{
	GameState::f_takeInput(dt, window);

	if (!m_windowHasFocus)
	{
		m_gamePaused = true;
		return;
	}
		

	static bool cam3d = true;

	if (m_gamePaused)//=============================================================Ingame menu options input (by pressing escape)
	{
		m_ingameOptionsDialog.f_update(dt, window);

		if (SFML_utils::onKeyPress(Controls::ACCEPT))
		{
			m_ingameOptionsDialog.f_activateSelectedOption();
			m_gamePaused = false;//if an option is selected this will always happen, so no condition for RESUME
		}
	}

	if (m_player.f_dyingAnimationFinished())//===================================================================================dead menu input
	{
		m_deadDialog.f_update(dt, window);
		if (SFML_utils::onKeyPress(Controls::ACCEPT))
			m_deadDialog.f_activateSelectedOption();

	}


	if (SFML_utils::onKeyPress(sf::Keyboard::C))
	{
		if (cam3d)
			m_camera.set2D();
		else
			m_camera.set3D();

		cam3d = !cam3d;

	}
	if (SFML_utils::onKeyPress(sf::Keyboard::P))
	{
		static bool persp = true;
		if (persp)
			m_camera.setOrthographic();
		else
			m_camera.setPerspective();

		persp = !persp;
	}

	if (SFML_utils::onKeyPress(sf::Keyboard::Escape))
	{
		if (!m_player.f_isDead() && !m_level->f_complete())
			m_gamePaused = !m_gamePaused;
	}
}

void PlayState3d::f_setLevel(Level* l)
{
	if (m_level == l)
		return;
	else
	{
		m_level = l;
		m_player.f_setLevel(m_level);
		m_player.f_setPos(m_level->f_getStartPos());
		m_player.f_reset();		
	}
}