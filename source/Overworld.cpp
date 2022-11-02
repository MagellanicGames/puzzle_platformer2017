#include "..\include\Overworld.h"
#include "..\include\SpriteGL.h"
#include "..\include\AnimationGL.h"
#include "..\include\Drawing.h"
#include "..\include\Shader.h"
#include "..\include\OverworldPlayer.h"
#include <iostream>
#include <glm\gtc\type_ptr.hpp>



Overworld::Overworld(MapNode* root /*= nullptr*/)
	:
	m_root(root),
	m_worldName("Not set")
{

	 m_sunColor = glm::vec3(0.5, 0.5, 0.5);
	 m_sunPos = glm::vec3(0.0f, 10.0f, 0.0f);

	m_MAPSIZE = vec(Dim::TILE_SIZE_S.x * 10.0f, Dim::TILE_SIZE_S.y * 10.0f);

	m_nodes["level1"] = m_root;
	m_currentNode = m_root;
	

	m_mapBase.reset(new AnimationGL(Drawing::m_mapBasesTexArray,Dir::TOP));
	m_mapBase->f_loadFrames(0, 0, 4);
	m_mapBase->f_set_position(vecf(0.0f, -Dim::TILE_SIZE_S.y));
	m_mapBase->f_set_scale(glm::vec3(m_MAPSIZE.x,1.0f,m_MAPSIZE.y));
	m_mapBase->f_start_animation(true);

	

	AnimationGL incompleteFlag(Drawing::m_mapActivesTexArray);
	incompleteFlag.f_loadFrames(0, 6, 5);
	incompleteFlag.f_start_animation(true);
	incompleteFlag.f_set_name("incomplete");
	incompleteFlag.f_set_frametime(0.15f);
	m_flagAnimations[incompleteFlag.f_getName()] = incompleteFlag;

	AnimationGL completeFlag(Drawing::m_mapActivesTexArray);
	completeFlag.f_loadFrames(0, 7, 5);
	completeFlag.f_start_animation(true);
	completeFlag.f_set_name("complete");
	completeFlag.f_set_frametime(0.015f);
	m_flagAnimations[completeFlag.f_getName()] = completeFlag;


	m_root->setAnimation(m_flagAnimations["incomplete"]);

	//0 mountaint, 1 mountain, 2 small cactus, 3 large cactus

	int mountain1 = 0;
	int mountain2 = 1;
	int smallCactus = 2;
	int largeCactus = 3;

	f_addDoodad(mountain1, vec(0, 0));
	f_addDoodad(smallCactus, vec(1, 0));
	f_addDoodad(largeCactus, vec(6, 0));
	f_addDoodad(mountain1, vec(7, 0));
	f_addDoodad(mountain2, vec(8, 0));

	f_addDoodad(mountain1, vec(9, 1));

	f_addDoodad(largeCactus, vec(0, 2));
	f_addDoodad(largeCactus, vec(7, 2));
	f_addDoodad(smallCactus, vec(9, 2));

	f_addDoodad(mountain2, vec(0, 3));
	f_addDoodad(mountain1, vec(1, 3));
	f_addDoodad(mountain2, vec(9, 3));

	f_addDoodad(mountain2, vec(1, 4));

	f_addDoodad(smallCactus, vec(6, 7));

	f_addDoodad(mountain2, vec(3, 8));

	f_addDoodad(largeCactus, vec(1, 9));
	f_addDoodad(mountain1, vec(2, 9));
	f_addDoodad(mountain2, vec(4, 9));
	f_addDoodad(mountain2, vec(9, 9));

	f_setLighting(); //also set in add node


}


void Overworld::f_setLighting()
{
	m_mapBase->f_setAffectedByLight(true);

	for (auto & d : m_doodads)
		d.f_setAffectedByLight(true);

	for (auto & n : m_nodes)
		n.second->getAnimation()->f_setAffectedByLight(true);
}
Overworld::~Overworld()
{}

void Overworld::f_update(float dt)
{
	static float angle = 90.0f; //0 dawn, 180 dusk
	
	std::cout << "Sun Angle: " << angle << "\n";
	if (angle > 360.0f)
		angle = 0.0f;
	angle += 1.0f * dt;
	m_sunPos.x = 10.0f * cosf(glm::radians(angle));
	m_sunPos.y = 10.0f * sinf(glm::radians(angle));

	m_mapBase->f_update(dt);

	for (auto & iter : m_nodes)
	{
		if (iter.second->f_levelComplete() && iter.second->getAnimation()->f_getName() != "complete")
		{
			std::cout << "setting animation to complete\n";
			iter.second->setAnimation(m_flagAnimations["complete"]);
		}
			
		iter.second->getAnimation()->f_update(dt);
	}

	f_sortDrawables();
}


void Overworld::f_draw(ptr<Shader>& shader)
{

	glUniform3fv(shader->uniform("sunPos"), 1, glm::value_ptr(m_sunPos));
	glUniform3fv(shader->uniform("sunColor"), 1, glm::value_ptr(m_sunColor));		

	m_mapBase->f_draw(shader);

	for (auto & d : m_drawOrder)
		d->f_draw(shader);
}


void Overworld::f_addLevelNode(MapNode* nodeToAdd, std::string levelNameOfParent, NodeID nodeID/*Left,right,up or down child of parent*/)
{
	if (m_nodes.find(levelNameOfParent) != m_nodes.end())//if the level name of parent is found, proceed to try and add
	{
		if (m_nodes.find(nodeToAdd->f_getLevelName()) == m_nodes.end())//if the level name is not already used, add
		{
			MapNode* node = m_nodes[levelNameOfParent];
			nodeToAdd->setAnimation(m_flagAnimations["incomplete"]);
			if (nodeID == NodeID::UPNODE)
			{
				node->f_setUp(nodeToAdd);
				m_nodes[nodeToAdd->f_getLevelName()] = node->f_up();
				nodeToAdd->f_setDown(node);
				
			}
			else if (nodeID == NodeID::DOWNNODE)
			{
				node->f_setDown(nodeToAdd);
				m_nodes[nodeToAdd->f_getLevelName()] = node->f_down();
				nodeToAdd->f_setUp(node);
			}
			else if (nodeID == NodeID::LEFTNODE)
			{
				node->f_setLeft(nodeToAdd);
				m_nodes[nodeToAdd->f_getLevelName()] = node->f_left();
				nodeToAdd->f_setRight(node);
			}
			else if (nodeID == NodeID::RIGHTNODE)
			{
				node->f_setRight(nodeToAdd);
				m_nodes[nodeToAdd->f_getLevelName()] = node->f_right();
				nodeToAdd->f_setLeft(node);
			}
			else
			{
				std::cout << "Invalid node id enum. Must be a direction\n";
				return;
			}				
		}
		else
		{
			std::cout << "Node with that level name already exists\n";
		}		

	}
	else
	{
		std::cout << "Parent not found\n";
	}

	f_setLighting();
}

MapNode* Overworld::f_moveToNextNode(Dir direction) //if not null returns the node to set the player position
{
	switch (direction)
	{
		case Dir::UP:
		{
			if (m_currentNode->f_up() != nullptr)
			{
				m_currentNode = m_currentNode->f_up();
				return m_currentNode;
			}
			break;
		}
		case Dir::DOWN:
		{
			if (m_currentNode->f_down() != nullptr)
			{
				m_currentNode = m_currentNode->f_down();
				return m_currentNode;
			}
			break;
		}
		case Dir::LEFT:
		{
			if (m_currentNode->f_left() != nullptr)
			{
				m_currentNode = m_currentNode->f_left();
				return m_currentNode;
			}
			break;
		}
		case Dir::RIGHT:
		{
			if (m_currentNode->f_right() != nullptr)
			{
				m_currentNode = m_currentNode->f_right();
				return m_currentNode;
			}
			break;
		}

	}

	return nullptr;
}

MapNode* Overworld::f_getNode(std::string levelName)
{
	if (m_nodes.find(levelName) != m_nodes.end())
		return m_nodes[levelName];
	else
	{
		std::cout << "Node with name \"" << levelName << "\" not found\n";
		return nullptr;
	}
	
}

void Overworld::f_addDoodad(int indexOfDoodad, vec pos) //hard coded for level1
{
	SpriteGL doodad(Drawing::m_mapActivesTexArray, indexOfDoodad);
	f_setDoodadPosition(doodad, pos);
	doodad.f_set_rotation(0, 0, 0);
	m_doodads.push_back(doodad);
}

void Overworld::f_setDoodadPosition(SpriteGL & doodad, vec pos) //position in map co-ordinates ie: 0,0 top left tile
{
	glm::vec3 position;	
	glm::vec3 offset(-Dim::TILE_SIZE_S.x * 0.5f, -(Dim::TILE_SIZE_S.y * 0.5), -Dim::TILE_SIZE_S.y * 0.5f);
	position.x = ((pos.x+1 )* Dim::TILE_SIZE_S.x) - m_MAPSIZE.x * 0.5f;
	position.y = 0.0f;
	position.z = ((pos.y+1 )* Dim::TILE_SIZE_S.y) - m_MAPSIZE.y * 0.5f;
	doodad.f_set_position(position + offset);
}

void Overworld::f_storeNodesForOrdering(OverworldPlayer& p)
{
	m_drawOrder.clear();

	m_drawOrder.push_back(p.getSprite());
	
	for (auto & n : m_nodes)
	{
		m_drawOrder.push_back(n.second->getAnimation());
	}

	for (auto& d : m_doodads)
		m_drawOrder.push_back(&d);

	f_sortDrawables();
	
}

void Overworld::f_sortDrawables()
{
	std::sort(m_drawOrder.begin(), m_drawOrder.end(), [](DrawableGL* a, DrawableGL* b)
	{
		return a->f_posGLM().z < b->f_posGLM().z;
	});
}

MapNode::MapNode(std::string levelName, Dir dirFromPrevNode, vec mapPos2d/* = vec()*/,MapNode* parent/*= nullptr*/, NodeID type /*= levelnode*/)
	:
	m_left(nullptr),
	m_right(nullptr),
	m_up(nullptr),
	m_down(nullptr),
	m_levelName(levelName),
	m_pos(vec3f()),
	m_type(type),
	m_levelComplete(false),
	m_2dMapPos(mapPos2d)
{
	m_pos.x = (Dim::TILE_SIZE_S.x*m_2dMapPos.x) + (Dim::TILE_SIZE_S.x * 0.5f);
	m_pos.y = -(Dim::TILE_SIZE_S.y - 2) + (Dim::TILE_SIZE_S.y *0.5f);
	m_pos.z = (Dim::TILE_SIZE_S.y*m_2dMapPos.y) + (Dim::TILE_SIZE_S.y * 0.5f);

	m_pos = m_pos - vec3f(Dim::TILE_SIZE_S.x*5.0f, 0.0f, Dim::TILE_SIZE_S.y*5.0f);
}

MapNode::~MapNode()
{

}

bool MapNode::f_movePlayer(Dir direction) //returns if was able to move player
{
	MapNode* node = nullptr;
	switch (direction)
	{
		case Dir::UP: node = m_up; break;
		case Dir::DOWN: node = m_down; break;
		case Dir::LEFT: node = m_left; break;
		case Dir::RIGHT: node = m_right; break;
	}

	if (node == nullptr)
		return false;

	if ((node->f_levelComplete()) ||
		(this->f_levelComplete() && node->f_levelComplete() == false))
	{		
		return true;
	}
	else
		return false;
	
}

glm::vec3 MapNode::f_posGLM()
{
	return glm::vec3(m_pos.x, m_pos.y, m_pos.z);
}

void MapNode::f_setUp(MapNode* node)
{
	m_up = node;
}

void MapNode::f_setDown(MapNode* node)
{
	m_down = node;
}
void MapNode::f_setLeft(MapNode* node)
{
	m_left = node;
}
void MapNode::f_setRight(MapNode* node)
{
	m_right = node;
}

void MapNode::setAnimation(AnimationGL & a)
{
	 m_animation = AnimationGL(a); 
	 m_animation.f_set_position(f_posGLM() + glm::vec3(8, 0, 0));
}