#include "Rig.h"

#include "gl/freeglut.h"

#include "../calculations/Math.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"

Rig::Rig(Math::vec3d pos, Math::vec3d rot, Math::vec3d scale)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;

	this->centre = nullptr;
}

Rig::~Rig()
{
	clearNodes();
}

void Rig::clearNodes()
{
	for (std::vector< Node* >::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		delete (*it);
	}

	nodes.clear();

	if (centre != nullptr)
		delete centre;
}

void Rig::rigFemaleElf()
{
	//Cleans up memory in case the rig is being used for a different model
	clearNodes();

	Math::vec3d _pos = { 0.0f, 0.0f, 0.0f };
	Math::vec3d _rot = { 0.0f, 0.0f, 0.0f };
	centre = new Node(_pos, _rot);
		
	/* HEAD
	*/
	_pos = { -0.01437f, 1.34524f, 0 };
	Node* neck = new Node("neck", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head_neck.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_head.png"),
		_pos, _rot);

	_pos = { 0, 0.17864f, 0 };
	Node* head = new Node("head", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_head.png"),
		_pos, _rot);
	

	/* BODY
	*/	
	_pos = { -0.01437f, 1.77437f, 0 };
	Node* torso = new Node("tor",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_torso.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_torso.png"),
		_pos, _rot);
	
	_pos = { 0.001553,0.03752, -0.073612 };
	Node* lower_body = new Node("lower_bod", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_skirt.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);

	
	/* ARMS	
	*/
	_pos = { 0.380106f, 1.20884f, -0.099151 };
	Node* leftarm_upper = new Node("la_u",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_top.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"),
		_pos, _rot);
	
	_pos = { 0.2, -0.67096, 0.05 };
	Node* leftarm_lower = new Node("la_l",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_bottom.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"),
		_pos, _rot);
	

	_pos = { -0.379216f, 1.21166f,  -0.097682f };	
	Node* rightarm_upper = new Node("ra_u",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_top.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"),
		_pos, _rot);
	
	_pos = { -0.21f, -0.67096f, 0.0};
	Node* rightarm_lower = new Node("ra_l", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_bottom.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"), 
		_pos, _rot);

	
	/* LEGS	
	*/	
	_pos = { 0.277095, -0.55379f, 0 };
	Node* leftleg_upper = new Node("ll_u", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_left.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);
		
	_pos = { 0.08f, -0.84f, 0 };
	Node* leftleg_lower = new Node("ll_l", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_left.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);


	_pos = { -0.184185f, -0.50577f, 0 };
	Node* rightleg_upper = new Node("rl_u", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_right.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);
		
	_pos = { -0.03f, -0.88f, 0 };
	Node* rightleg_lower = new Node("rl_l", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_right.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);


	//Setting the parent/child relations
	neck->addChild(head);

	leftarm_upper->addChild(leftarm_lower);
	rightarm_upper->addChild(rightarm_lower);

	leftleg_upper->addChild(leftleg_lower);
	rightleg_upper->addChild(rightleg_lower);

	torso->addChild(neck);
	torso->addChild(lower_body);
	torso->addChild(leftarm_upper);
	torso->addChild(rightarm_upper);
	torso->addChild(leftleg_upper);
	torso->addChild(rightleg_upper);

	centre->addChild(torso);


	//Storing all nodes to simplify the search process for getting nodes
	nodes.push_back(neck);
	nodes.push_back(head);

	nodes.push_back(torso);
	nodes.push_back(lower_body);
	
	nodes.push_back(leftarm_upper);
	nodes.push_back(leftarm_lower);
	nodes.push_back(rightarm_upper);
	nodes.push_back(rightarm_lower);
	
	nodes.push_back(leftleg_upper);
	nodes.push_back(rightleg_upper);
	nodes.push_back(leftleg_lower);
	nodes.push_back(rightleg_lower);

}

void Rig::drawRig()
{

	glScalef(scale.x, scale.y, scale.z);	

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);
	
	centre->draw();
}

Node* Rig::getNode(std::string node_name)
{
	for (Node* node : nodes) {
		if (node->getName() == node_name)
			return node;
	}

	return nullptr;
}

void Rig::setRotation(Math::vec3d rot)
{
	this->rot = rot;
}

void Rig::setPosition(Math::vec3d pos)
{
	this->pos = pos;
}

void Rig::setScale(Math::vec3d scale)
{
	this->scale = scale;
}

