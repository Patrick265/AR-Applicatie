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
	Rig();
	~Rig();
	
	void initRig(Math::vec3d pos, Math::vec3d rot, Math::vec3d scale);


	/*
		Rigs the model of a female elf
	*/
	void rigFemaleElf();

	/*
	Rigs the model of a goblin (an enemy)
	*/
	void rigGoblin();


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

	

private:

	/*
	Frees all the pointers and then clears the vector
*/
	void clearNodes();

	/*
		Transforms Blender coordinates into OpenGL coordinates

		@param posCords - The position coordinates from blender

		Returns the position coordinates in OpenGL
		*/
	Math::vec3d convertCoordinates(Math::vec3d posCords);


	/*
		Transforms Blender coordinates into OpenGL coordinates with a parent

		@param posCords - The position coordinates from blender
		@param parent - the position coordinates from the parent

		Returns the position coordinates in OpenGL
		*/
	Math::vec3d convertCoordinates(Math::vec3d posCords, Math::vec3d parent);
};