#include "Rig.h"

#include "gl/freeglut.h"

#include "../calculations/Math.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"

Rig::Rig(std::string rig_type, Math::vec3d pos, Math::vec3d rot, Math::vec3d scale)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;

	this->centre = nullptr;

	if (rig_type == "elf")
		rigFemaleElf();
	else if (rig_type == "goblin")
		rigGoblin();
	
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
	Math::vec3d _pos = { 0.0f, 0.0f, 0.0f };
	Math::vec3d _rot = { 0.0f, 0.0f, 0.0f };
	centre = new Node(_pos, _rot);

	Math::vec3d torso = { 0.0f, 0.0f, 0.952426f };
		Math::vec3d arm_left_top = { 0.404859f , 0.120226f ,2.16489f };
			Math::vec3d arm_left_bottom = { 0.61229f , 0.068894f ,1.45316f };
		Math::vec3d arm_right_top = { -0.38416f, 0.119478f , 2.16327f };
			Math::vec3d arm_right_bottom = { -0.605497f, 0.112396f, 1.48285f };
		Math::vec3d leg_left_top = { 0.194586f, 0.0f, 0.735892f };
			Math::vec3d leg_left_bottom = { 0.272154f, 0.0f, -0.104308f};
		Math::vec3d leg_right_top = { -0.226518f, 0.0, 0.742508f };
			Math::vec3d leg_right_bottom = { -0.257862f, 0.0f, -0.13729f};
		Math::vec3d neck = { 0.015737f,0.0f, 2.3002f };
			Math::vec3d head = { 0.0f,0.0f,2.48477f };
		Math::vec3d skirt = { -0.002984f, 0.08143f,1.03797f };
		Math::vec3d big_sack = {0.0f,0.0f, 0.32386f };

		
	/* HEAD
	*/
	_pos = convertCoordinates(neck, torso);
	Node* elf_neck = new Node("neck", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head_neck.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_head.png"),
		_pos, _rot);

	_pos = convertCoordinates(head, neck);
	Node* elf_head = new Node("head", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_head.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_head.png"),
		_pos, _rot);
	

	/* BODY
	*/	

	_pos = convertCoordinates(torso);
	Node* elf_torso = new Node("torso",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_torso.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_torso.png"),
		_pos, _rot);
	
	_pos = convertCoordinates(skirt, torso);
	Node* elf_lower_body = new Node("lower_bod", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_skirt.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);

	
	/* ARMS	
	*/
	_pos = convertCoordinates(arm_left_top, torso);
	Node* elf_leftarm_upper = new Node("la_u",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_top.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"),
		_pos, _rot);
	
	_pos = convertCoordinates(arm_left_bottom, arm_left_top);
	Node* elf_leftarm_lower = new Node("la_l",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_left_bottom.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"),
		_pos, _rot);
	

	_pos = convertCoordinates(arm_right_top, torso);
	Node* elf_rightarm_upper = new Node("ra_u",ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_top.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"),
		_pos, _rot);
	
	_pos = convertCoordinates(arm_right_bottom, arm_right_top);
	Node* elf_rightarm_lower = new Node("ra_l", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_arm_right_bottom.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_arms.png"), 
		_pos, _rot);

	
	/* LEGS	
	*/	
	_pos = convertCoordinates(leg_left_top, torso);
	Node* elf_leftleg_upper = new Node("ll_u", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_left.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);
		
	_pos = convertCoordinates(leg_left_bottom, leg_left_top);
	Node* elf_leftleg_lower = new Node("ll_l", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_left.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);

	_pos = convertCoordinates(leg_right_top, torso);
	Node* elf_rightleg_upper = new Node("rl_u", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_top_right.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);
		
	_pos = convertCoordinates(leg_right_bottom, leg_right_top);
	Node* elf_rightleg_lower = new Node("rl_l", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_leg_bottom_right.obj"),
		TextureHandler::addTexture("Resources/Rune/npc_legs.png"),
		_pos, _rot);

	_pos = convertCoordinates(big_sack, torso);
	Node* elf_big_sack = new Node("sack", ObjLoader::loadObj("Resources/Rigid_NPC/NPC_big_sack.obj"),
		TextureHandler::addTexture("Resources/Rigid_NPC/NPC_big_sack.png"),
		_pos, _rot);



	//Setting the parent/child relations
	elf_neck->addChild(elf_head);

	elf_leftarm_upper->addChild(elf_leftarm_lower);
	elf_rightarm_upper->addChild(elf_rightarm_lower);
	elf_leftleg_upper->addChild(elf_leftleg_lower);
	elf_rightleg_upper->addChild(elf_rightleg_lower);

	elf_torso->addChild(elf_neck);
	elf_torso->addChild(elf_lower_body);
	elf_torso->addChild(elf_leftarm_upper);
	elf_torso->addChild(elf_rightarm_upper);
	elf_torso->addChild(elf_leftleg_upper);
	elf_torso->addChild(elf_rightleg_upper);
	elf_torso->addChild(elf_big_sack);

	centre->addChild(elf_torso);


	//Storing all nodes to simplify the search process for getting nodes
	nodes.push_back(elf_neck);
	nodes.push_back(elf_head);
	nodes.push_back(elf_big_sack);

	nodes.push_back(elf_torso);
	nodes.push_back(elf_lower_body);
	
	nodes.push_back(elf_leftarm_upper);
	nodes.push_back(elf_leftarm_lower);
	nodes.push_back(elf_rightarm_upper);
	nodes.push_back(elf_rightarm_lower);
	
	nodes.push_back(elf_leftleg_upper);
	nodes.push_back(elf_rightleg_upper);
	nodes.push_back(elf_leftleg_lower);
	nodes.push_back(elf_rightleg_lower);

}

void Rig::rigGoblin()
{
	Math::vec3d _pos = { 0.0f, 0.0f, 0.0f };
	Math::vec3d _rot = { 0.0f, 0.0f, 0.0f };
	Math::vec3d parentPos;
	centre = new Node(_pos, _rot);

	Math::vec3d torso = { 0.0f, 0.0f, 5.50018f };
		Math::vec3d arm_left_top = { 1.42841f , -0.220004f ,8.22543f };
			Math::vec3d arm_left_bottom = { 2.05847f ,-0.30099f ,6.33814f };
		Math::vec3d arm_right_top = { -1.43497f,-0.222508f , 8.26748f };
			Math::vec3d arm_right_bottom = { -2.16087f, -0.321007f, 6.32603f };
		Math::vec3d leg_left_top = { 0.466519f, 0.368012f, 5.4683f};
			Math::vec3d leg_left_bottom = { 0.5619f, 0.169499f, 3.51627f};
		Math::vec3d leg_right_top = { -0.562285f, 0.449695f, 5.47866f};
			Math::vec3d leg_right_bottom = { -0.582013f, 0.280366f, 3.42471f};

	//Torso
	_pos = convertCoordinates(torso);
	Node* goblin_torso = new Node("torso", ObjLoader::loadObj("Resources/Enemy/Goblin_torso.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_torso.png"),
		_pos, _rot);

	//goblin arm left top
	_pos = convertCoordinates(arm_left_top, torso);
	Node* goblin_arm_left_top = new Node("la_u", ObjLoader::loadObj("Resources/Enemy/Goblin_arm_left_top.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_arm_left_top.png"),
		_pos, _rot);

	//goblin arm right top
	_pos = convertCoordinates(arm_right_top, torso);
	Node* goblin_arm_right_top = new Node("ra_u", ObjLoader::loadObj("Resources/Enemy/Goblin_arm_right_top.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_arm_top_right.png"),
		_pos, _rot);

	//goblin leg left top
	_pos = convertCoordinates(leg_left_top, torso);
	Node* goblin_leg_left_top = new Node("ll_u", ObjLoader::loadObj("Resources/Enemy/Goblin_leg_left_top.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_top.png"),
		_pos, _rot);

	//goblin leg right top
	_pos = convertCoordinates(leg_right_top, torso);
	Node* goblin_leg_right_top = new Node("rl_u", ObjLoader::loadObj("Resources/Enemy/Goblin_leg_right_top.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_top.png"),
		_pos, _rot);

	//goblin arm left bottom
	_pos = convertCoordinates(arm_left_bottom, arm_left_top);
	Node* goblin_arm_left_bottom = new Node("la_l", ObjLoader::loadObj("Resources/Enemy/Goblin_arm_left_bottom.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_arm_left_bottom.png"),
		_pos, _rot);

	//goblin arm right bottom
	_pos = convertCoordinates(arm_right_bottom, arm_right_top);
	Node* goblin_arm_right_bottom = new Node("ra_l", ObjLoader::loadObj("Resources/Enemy/Goblin_arm_right_bottom.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_arm_right_bottom.png"),
		_pos, _rot);

	//goblin leg left bottom
	_pos = convertCoordinates(leg_left_bottom, leg_left_top);
	Node* goblin_leg_left_bottom = new Node("ll_l", ObjLoader::loadObj("Resources/Enemy/Goblin_leg_left_bottom.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_leg_left_bottom.png"),
		_pos, _rot);

	//goblin leg right bottom
	_pos = convertCoordinates(leg_right_bottom, leg_right_top);
	Node* goblin_leg_right_bottom = new Node("rl_l", ObjLoader::loadObj("Resources/Enemy/Goblin_leg_right_bottom.obj"),
		TextureHandler::addTexture("Resources/Enemy/goblin_leg_right_bottom.png"),
		_pos, _rot);


	goblin_arm_left_top->addChild(goblin_arm_left_bottom);
	goblin_arm_right_top->addChild(goblin_arm_right_bottom);
	goblin_leg_left_top->addChild(goblin_leg_left_bottom);
	goblin_leg_right_top->addChild(goblin_leg_right_bottom);

	goblin_torso->addChild(goblin_arm_left_top);
	goblin_torso->addChild(goblin_arm_right_top);
	goblin_torso->addChild(goblin_leg_left_top);
	goblin_torso->addChild(goblin_leg_right_top);

	centre->addChild(goblin_torso);

	nodes.push_back(goblin_torso);

	nodes.push_back(goblin_arm_right_top);
	nodes.push_back(goblin_arm_right_bottom);
	nodes.push_back(goblin_arm_left_top);
	nodes.push_back(goblin_arm_left_bottom);

	nodes.push_back(goblin_leg_left_bottom);
	nodes.push_back(goblin_leg_right_bottom);
	nodes.push_back(goblin_leg_left_top);
	nodes.push_back(goblin_leg_right_top);
}

Math::vec3d Rig::convertCoordinates(Math::vec3d posCords)
{
	Math::vec3d convertedCords;
	convertedCords.x = posCords.x;
	convertedCords.y = posCords.z;
	convertedCords.z = -posCords.y;
	return convertedCords;
}

Math::vec3d Rig::convertCoordinates(Math::vec3d posCords, Math::vec3d parent)
{
	Math::vec3d convertedCords;
	convertedCords.x = posCords.x - parent.x;
	convertedCords.y = posCords.z - parent.z;
	convertedCords.z = -posCords.y - (-parent.y);

	return convertedCords;
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

