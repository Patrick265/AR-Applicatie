#pragma once

#include "Node.h"

class Rig {

private:
	Math::vec3d pos;
	Math::vec3d rot;
	Math::vec3d scale;

	//Node at the centre of the model
	Node* centre;
	//All nodes in the rig
	std::vector<Node*> nodes;

public:
	/*
		Constructs a rig with a position, rotation and scale
	*/
	Rig(Math::vec3d pos, Math::vec3d rot, Math::vec3d scale);
	~Rig();

	/*
		Frees all the pointers and then clears the vector
	*/
	void clearNodes();

	/*
		Rigs the model of a female elf
	*/
	void rigFemaleElf();

	/*
		Draws all the components of the rig
	*/
	void drawRig();

	/*
		Retrieves the node whose name matches the given string

		@param node_name - The given name

		Returns the Node pointer that has the given name
	*/
	Node* getNode(std::string node_name);

	void setRotation(Math::vec3d rot);
	void setPosition(Math::vec3d pos);
	void setScale(Math::vec3d scale);
};