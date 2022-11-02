#include "Common.h"
#include <glm\vec3.hpp>
#include "AnimationGL.h"

class Shader;
class SpriteGL;

class DrawableGL;
class OverworldPlayer;

enum NodeID{LEFTNODE,RIGHTNODE, UPNODE,DOWNNODE,LEVELNODE,NAVNODE};

/*

		root
	//			\\
 left			right
			//			\\
		left		   right

*/
class MapNode
{
public:

	MapNode(std::string levelName, Dir dirFromPreviousNode,vec MapPos2d = vec(),MapNode* parent = nullptr,NodeID nodeType = NodeID::LEVELNODE);
	~MapNode();

	void		f_setLevelName(std::string name) { m_levelName = name; }
	void		f_setPos(vec3f v) { m_pos = v; }
	std::string f_getLevelName()const { return m_levelName; }

	MapNode*	f_left() { return m_left; }
	MapNode*	f_right() { return m_right; }
	MapNode*	f_up(){return m_up;}
	MapNode*	f_down(){return m_down;}

	bool f_movePlayer(Dir direction); //returns if was able to move


	void f_setLeft(MapNode* node);
	void f_setRight(MapNode* node);
	void f_setUp(MapNode* node);
	void f_setDown(MapNode* node);

	NodeID f_type()const { return m_type; }

	glm::vec3	f_posGLM();
	vec3f		f_pos()const { return m_pos; }

	void		f_setLevelComplete(bool b) { m_levelComplete = b; }
	bool		f_levelComplete()const { return m_levelComplete; }

	AnimationGL* getAnimation() { return &m_animation; }
	void		setAnimation(AnimationGL & a);

private:


	MapNode*	m_left;
	MapNode*	m_right;
	MapNode*	m_up;
	MapNode*	m_down;
	std::string m_levelName;//reference for level store
	NodeID		m_type;
	vec3f		m_pos;
	vec			m_2dMapPos;
	bool		m_levelComplete;
	AnimationGL m_animation;
	
};


class Overworld
{
public:

	Overworld(MapNode* root = nullptr);
	~Overworld();

	void		f_addLevelNode(MapNode* nodeToAdd,std::string levelNameOfParent, NodeID nodeID);
	void		f_setWorldName(std::string name) { m_worldName = name; }
	MapNode*	f_getNode(std::string levelName);
	MapNode*	f_getRoot() { return m_root; }
	MapNode*	f_getCurrentNode() { return m_currentNode; }
	MapNode*	f_moveToNextNode(Dir direction);

	virtual void f_draw(ptr<Shader> & shader);
	virtual void f_update(float dt);

	void f_storeNodesForOrdering(OverworldPlayer& p);

	vec			m_MAPSIZE;
private:

	void f_addDoodad(int indexOfDoodad, vec pos);
	void f_setDoodadPosition(SpriteGL & doodad,vec pos);
	void f_sortDrawables();
	void f_setLighting();

	int									m_numNodes;

	glm::vec3							m_sunColor;
	glm::vec3							m_sunPos;

	MapNode*							m_root;
	MapNode*							m_currentNode;//where the player currently sits
	std::string							m_worldName;
	std::map<std::string, MapNode*>		m_nodes;	
	ptr<AnimationGL>					m_mapBase;
	
	std::vector<SpriteGL>				m_doodads;
	std::map<std::string, AnimationGL>	m_flagAnimations;
	std::vector<DrawableGL*>			m_drawOrder;
};