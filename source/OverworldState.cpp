#include "..\include\OverworldState.h"
#include "..\include\Drawing.h"
#include "..\include\SpriteGL.h"
#include "..\include\Overworld.h"
#include "..\include\EventQueue.h"
#include <iostream>
#include <glm\gtc\type_ptr.hpp>

OverworldState::OverworldState()
	:
	m_camera(glm::vec3(),Dim::RES_X,Dim::RES_Y),
	m_inMenu(false)
{
	
	//m_player->f_set_rotation(-45, 0, 0);	
		
	m_camera.setOverworld();
	m_camera.focus(glm::vec3(0,0,Dim::TILE_SIZE_S.y * 2.5));
	m_overworld.reset(new Overworld(new MapNode("level1",Dir::UP, vec(2,4))));
	m_overworld->f_addLevelNode(new MapNode("level2", Dir::RIGHT,vec(2,1)), "level1", NodeID::UPNODE);
	m_overworld->f_addLevelNode(new MapNode("level3", Dir::RIGHT,vec(5, 1)), "level2", NodeID::RIGHTNODE);	
	m_overworld->f_addLevelNode(new MapNode("nav1", Dir::DOWN, vec(8,1),nullptr,NodeID::NAVNODE), "level3", NodeID::RIGHTNODE);
	//m_overworld->f_addLevelNode(new MapNode("level4", Dir::RIGHT, m_overworld->f_getNode("nav1")->f_pos() + vec3f(0, 0, Dim::TILE_SIZE_S.x * 5)), "nav1", NodeID::DOWNNODE);

	

	m_player.setPos(m_overworld->f_getCurrentNode()->f_posGLM() + glm::vec3(0,0,4));	

	m_overworld->f_storeNodesForOrdering(m_player);

	m_currentLevelName = m_overworld->f_getCurrentNode()->f_getLevelName();
	EventQueue::f_addEvent(Event::CHANGELEVEL);
}

OverworldState::~OverworldState()
{}

void OverworldState::f_notifyOfLevelCompleted()
{
	m_overworld->f_getCurrentNode()->f_setLevelComplete(true);
}

void OverworldState::f_run(float dt, sf::RenderWindow& window)
{
	GameState::f_pollEvents(dt, window);
	GameState::f_run(dt, window);

	f_update(dt, window);	
	GameState::f_draw(dt, window);	
	f_takeInput(dt, window);
	

	
}

void OverworldState::f_update(float dt, sf::RenderWindow & window)
{
	if (m_inMenu)	
		m_overworldDialog.f_update(dt, window);
	m_overworld->f_update(dt);
	m_player.update(dt);
	m_overworld->f_storeNodesForOrdering(m_player);
}

void OverworldState::f_drawGL(float dt, ptr<Shader> & shader)
{
	m_camera.passParameters(shader);
	m_overworld->f_draw(shader); //draws player as well
	
}
void OverworldState::f_drawSFML(float dt, sf::RenderWindow & window)
{
	Text::draw(vecf(50, 50), 48, "Overworld. " + m_overworld->f_getCurrentNode()->f_getLevelName(), window);
	
	window.draw(m_fadeRect);

	if (m_inMenu)
		m_overworldDialog.f_draw(dt, window);
}

void OverworldState::f_takeInput(float dt, sf::RenderWindow & window)
{
	GameState::f_takeInput(dt, window);
	if (!m_windowHasFocus)
		return;
	if (SFML_utils::onKeyPress(sf::Keyboard::Escape))
	{
		m_inMenu = !m_inMenu;
	}

	if (m_inMenu)
	{
		if (SFML_utils::onKeyPress(Controls::ACCEPT))
		{
			m_overworldDialog.f_activateSelectedOption();
			m_inMenu = false;//if an option is selected this will always happen, so no condition for RESUME
		}
	}

	if (m_inMenu)
		return;

	if (SFML_utils::onKeyPress(Controls::UP) && m_overworld->f_getCurrentNode()->f_movePlayer(Dir::UP))
	{
		f_updatePlayerPosition(Dir::UP);
	}

	if (SFML_utils::onKeyPress(Controls::DOWN) && m_overworld->f_getCurrentNode()->f_movePlayer(Dir::DOWN))
	{
		f_updatePlayerPosition(Dir::DOWN);
	}

	if (SFML_utils::onKeyPress(Controls::LEFT) && m_overworld->f_getCurrentNode()->f_movePlayer(Dir::LEFT))
	{
		f_updatePlayerPosition(Dir::LEFT);
	}
	if (SFML_utils::onKeyPress(Controls::RIGHT) && m_overworld->f_getCurrentNode()->f_movePlayer(Dir::RIGHT))
	{
		f_updatePlayerPosition(Dir::RIGHT);
	}

	if (SFML_utils::onKeyPress(Controls::ACCEPT))
	{
		if (m_overworld->f_getCurrentNode()->f_type() == NodeID::LEVELNODE)
		{
			std::cout << "Load level " << m_overworld->f_getCurrentNode()->f_getLevelName() << "\n";
			m_currentLevelName = m_overworld->f_getCurrentNode()->f_getLevelName();
			EventQueue::f_addEvent(Event::CHANGELEVEL);
			EventQueue::f_addEvent(Event::LEVELSTARTSCREEN);
		}

		else
			std::cout << "Cannot load level, is a navnode with id: " << m_overworld->f_getCurrentNode()->f_getLevelName() << "\n";
	}
}

void OverworldState::f_updatePlayerPosition(Dir direction)
{
	if (m_player.isMoving())
		return;

	MapNode* node = nullptr;
	node = m_overworld->f_moveToNextNode(direction); //returns node if one is found in given direction
	m_player.setTargetPos(node->f_posGLM() + glm::vec3(0,0,4));//player needs to be slightly infront of level sprite
	/*if a nav node, to know if it is been visited already, it is set to being complete*/
	if (m_overworld->f_getCurrentNode()->f_type() == NodeID::NAVNODE)
		m_overworld->f_getCurrentNode()->f_setLevelComplete(true);
}