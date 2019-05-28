#include "Rig.h"

#include "gl/freeglut.h"

#include "../calculations/Math.h"
#include "../util/ObjLoader.h"
#include "../util/TextureHandler.h"

Rig::Rig(std::string rig_type, Math::vec3d rot, Math::vec3d scale)
	:rot(rot), scale(scale)
{
	centre = Node(Math::vec3d{ 0,0,0 }, Math::vec3d{ 0,0,0 });

	if (rig_type == "elf")
		rigFemaleElf();
	else if (rig_type == "goblin")
		rigGoblin();
}

Rig::Rig(const Rig & rig)
	:rot(rig.rot), scale(rig.scale), centre(rig.centre), nodes(rig.nodes)
{
}


void Rig::rigFemaleElf()
{
	Math::vec3d _pos = { 0.0f, 2.59547f, 0.0f };
	Math::vec3d _rot = { 0.0f, 0.0f, 0.0f };
	centre = Node(_pos, _rot);

	Math::vec3d torso = { 0.0f, 0.0f, 0.952426f };
	Math::vec3d arm_left_top = { 0.404859f , 0.120226f ,2.16489f };
	Math::vec3d arm_left_bottom = { 0.61229f , 0.068894f ,1.45316f };
	Math::vec3d arm_right_top = { -0.38416f, 0.119478f , 2.16327f };
	Math::vec3d arm_right_bottom = { -0.605497f, 0.112396f, 1.48285f };
	Math::vec3d leg_left_top = { 0.194586f, 0.0f, 0.735892f };
	Math::vec3d leg_left_bottom = { 0.272154f, 0.0f, -0.104308f };
	Math::vec3d leg_right_top = { -0.226518f, 0.0, 0.742508f };
	Math::vec3d leg_right_bottom = { -0.257862f, 0.0f, -0.13729f };
	Math::vec3d neck = { 0.015737f,0.0f, 2.3002f };
	Math::vec3d head = { 0.0f,0.0f,2.48477f };
	Math::vec3d skirt = { -0.002984f, 0.08143f,1.03797f };
	Math::vec3d big_sack = { 0.0f,0.0f, 0.32386f };

	Math::vec3d weapon_log = {0.667523f,-0.030498f, 0.71092f};
	Math::vec3d weapon_weights = {0.0f,0.0f,0.820919f};


	/* HEAD
	*/
	_pos = convertCoordinates(neck, torso);
	Node elf_neck("neck", "elf_neck",
		"elf_head",
		_pos, _rot);

	_pos = convertCoordinates(head, neck);
	Node elf_head("head", "elf_head",
		"elf_head",
		_pos, _rot);


	/* BODY
	*/

	_pos = convertCoordinates(torso);
	Node elf_torso("torso", "elf_torso",
		"elf_torso",
		_pos, _rot);

	_pos = convertCoordinates(skirt, torso);
	Node elf_lower_body("lower_bod", "elf_lowerbod",
		"elf_legs",
		_pos, _rot);


	/* ARMS
	*/
	_pos = convertCoordinates(arm_left_top, torso);
	Node elf_leftarm_upper("la_u", "elf_la_u",
		"elf_arms",
		_pos, _rot);

	_pos = convertCoordinates(arm_left_bottom, arm_left_top);
	Node elf_leftarm_lower("la_l", "elf_la_l",
		"elf_arms",
		_pos, _rot);


	_pos = convertCoordinates(arm_right_top, torso);
	Node elf_rightarm_upper("ra_u", "elf_ra_u",
		"elf_arms",
		_pos, _rot);

	_pos = convertCoordinates(arm_right_bottom, arm_right_top);
	Node elf_rightarm_lower("ra_l", "elf_ra_l",
		"elf_arms",
		_pos, _rot);


	/* LEGS
	*/
	_pos = convertCoordinates(leg_left_top, torso);
	Node elf_leftleg_upper("ll_u", "elf_ll_u",
		"elf_legs",
		_pos, _rot);

	_pos = convertCoordinates(leg_left_bottom, leg_left_top);
	Node elf_leftleg_lower("ll_l", "elf_rl_l",
		"elf_legs",
		_pos, _rot);

	_pos = convertCoordinates(leg_right_top, torso);
	Node elf_rightleg_upper("rl_u", "elf_rl_u",
		"elf_legs",
		_pos, _rot);

	_pos = convertCoordinates(leg_right_bottom, leg_right_top);
	Node elf_rightleg_lower("rl_l", "elf_rl_l",
		"elf_legs",
		_pos, _rot);

	_pos = convertCoordinates(big_sack, torso);
	Node elf_big_sack("sack", "elf_sack",
		"elf_sack",
		_pos, _rot);

		_pos = convertCoordinates(weapon_log, arm_left_bottom);
	Node elf_weapon("la_weapon", "none",
		"none",
		_pos, _rot);

	//Setting the parent/child relations
	elf_neck.addChild("head");

	elf_leftarm_lower.addChild("la_weapon");

	elf_leftarm_upper.addChild("la_l");
	elf_rightarm_upper.addChild("ra_l");
	elf_leftleg_upper.addChild("ll_l");
	elf_rightleg_upper.addChild("rl_l");

	elf_torso.addChild("neck");
	elf_torso.addChild("lower_bod");
	elf_torso.addChild("la_u");
	elf_torso.addChild("ra_u");
	elf_torso.addChild("ll_u");
	elf_torso.addChild("rl_u");
	elf_torso.addChild("sack");

	centre.addChild("torso");


	//Storing the nodes
	nodes["la_weapon"] = elf_weapon;

	nodes["neck"] = elf_neck;
	nodes["head"] = elf_head;
	nodes["sack"] = elf_big_sack;

	nodes["torso"] = elf_torso;
	nodes["lower_bod"] = elf_lower_body;

	nodes["la_u"] = elf_leftarm_upper;
	nodes["la_l"] = elf_leftarm_lower;
	nodes["ra_u"] = elf_rightarm_upper;
	nodes["ra_l"] = elf_rightarm_lower;

	nodes["ll_u"] = elf_leftleg_upper;
	nodes["ll_l"] = elf_leftleg_lower;
	nodes["rl_u"] = elf_rightleg_upper;
	nodes["rl_l"] = elf_rightleg_lower;


}

void Rig::rigGoblin()
{
	Math::vec3d _pos = { 0.0f, 5.50018f, 0.0f };
	Math::vec3d _rot = { 0.0f, 0.0f, 0.0f };
	//Math::vec3d parentPos;
	centre = Node(_pos, _rot);

	Math::vec3d torso = { 0.0f, 0.0f, 5.50018f };
	Math::vec3d arm_left_top = { 1.42841f , -0.220004f ,8.22543f };
	Math::vec3d arm_left_bottom = { 2.05847f ,-0.30099f ,6.33814f };
	Math::vec3d arm_right_top = { -1.43497f,-0.222508f , 8.26748f };
	Math::vec3d arm_right_bottom = { -2.16087f, -0.321007f, 6.32603f };
	Math::vec3d leg_left_top = { 0.466519f, 0.368012f, 5.4683f };
	Math::vec3d leg_left_bottom = { 0.5619f, 0.169499f, 3.51627f };
	Math::vec3d leg_right_top = { -0.562285f, 0.449695f, 5.47866f };
	Math::vec3d leg_right_bottom = { -0.582013f, 0.280366f, 3.42471f };
	Math::vec3d icicle_left = {1.93501f,0.399071f,3.80603f};
	Math::vec3d icicle_right = {-2.0828f,0.399071f,3.80603f};

	//Torso
	_pos = convertCoordinates(torso);
	Node goblin_torso("torso", "goblin_torso",
		"goblin_torso",
		_pos, _rot);

	//goblin arm left top
	_pos = convertCoordinates(arm_left_top, torso);
	Node goblin_arm_left_top("la_u", "goblin_la_u",
		"goblin_la_u",
		_pos, _rot);

	//goblin arm right top
	_pos = convertCoordinates(arm_right_top, torso);
	Node goblin_arm_right_top("ra_u", "goblin_ra_u",
		"goblin_ra_u",
		_pos, _rot);

	//goblin leg left top
	_pos = convertCoordinates(leg_left_top, torso);
	Node goblin_leg_left_top("ll_u", "goblin_ll_u",
		"goblin_ll_u",
		_pos, _rot);

	//goblin leg right top
	_pos = convertCoordinates(leg_right_top, torso);
	Node goblin_leg_right_top("rl_u", "goblin_rl_u",
		"goblin_ll_u",
		_pos, _rot);

	//goblin arm left bottom
	_pos = convertCoordinates(arm_left_bottom, arm_left_top);
	Node goblin_arm_left_bottom("la_l", "goblin_la_l",
		"goblin_la_l",
		_pos, _rot);

	//goblin arm right bottom
	_pos = convertCoordinates(arm_right_bottom, arm_right_top);
	Node goblin_arm_right_bottom("ra_l", "goblin_ra_l",
		"goblin_ra_l",
		_pos, _rot);

	//goblin leg left bottom
	_pos = convertCoordinates(leg_left_bottom, leg_left_top);
	Node goblin_leg_left_bottom("ll_l", "goblin_ll_l",
		"goblin_ll_l",
		_pos, _rot);

	//goblin leg right bottom
	_pos = convertCoordinates(leg_right_bottom, leg_right_top);
	Node goblin_leg_right_bottom("rl_l", "goblin_rl_l",
		"goblin_rl_l",
		_pos, _rot);

	_pos = convertCoordinates(icicle_left, arm_left_bottom);
		Node goblin_icicle_left("la_weapon", "icicle",
		"icicle",
		_pos, _rot);

		_pos = convertCoordinates(icicle_right, arm_right_bottom);
		Node goblin_icicle_right("ra_weapon", "icicle",
		"icicle",
		_pos, _rot);


		
	//Setting parent/child relations
	goblin_arm_left_bottom.addChild("la_weapon");
	goblin_arm_right_bottom.addChild("ra_weapon");

	goblin_arm_left_top.addChild("la_l");
	goblin_arm_right_top.addChild("ra_l");
	goblin_leg_left_top.addChild("ll_l");
	goblin_leg_right_top.addChild("rl_l");


	goblin_torso.addChild("la_u");
	goblin_torso.addChild("ra_u");
	goblin_torso.addChild("ll_u");
	goblin_torso.addChild("rl_u");

	centre.addChild("torso");


	//Adding nodes to map
	nodes["torso"] = goblin_torso;

	nodes["la_weapon"] = goblin_icicle_left;
	nodes["ra_weapon"] = goblin_icicle_right;

	nodes["la_u"] = goblin_arm_left_top;
	nodes["la_l"] = goblin_arm_left_bottom;
	nodes["ra_u"] = goblin_arm_right_top;
	nodes["ra_l"] = goblin_arm_right_bottom;

	nodes["ll_u"] = goblin_leg_left_top;
	nodes["ll_l"] = goblin_leg_left_bottom;
	nodes["rl_u"] = goblin_leg_right_top;
	nodes["rl_l"] = goblin_leg_right_bottom;
}

void Rig::drawRig(const std::map<std::string, Graphics::mesh> &meshes, const std::map<std::string, uint16_t> &textures)
{
	glPushMatrix();

	glScalef(scale.x, scale.y, scale.z);

	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);

	centre.draw(nodes, meshes, textures);

	glPopMatrix();
}

Node & Rig::getNode(std::string node_name)
{
	return nodes[node_name];
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


