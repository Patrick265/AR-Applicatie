#pragma once


#include <map>

#include "Node.h"


class Rig {

private:
	Math::vec3d rot;
	Math::vec3d scale;

	//Node at the centre of the model
	Node centre;
	//All nodes in the rig
	std::map<std::string, Node> nodes;

public:
	/*
		Constructs a rig with a position, rotation and scale

		@param rig_type - The rig that wil be constructed (currently elf or goblin)
		@param rot - The current rotation
		@param scale - The current scale

	*/
	Rig(std::string rig_type, Math::vec3d rot, Math::vec3d scale);
	Rig(const Rig &rig);

	//DONT USE, needed for default constructor
	Rig() {}
	~Rig() {}


	/*
		Draws all the components of the rig

		@param meshes - A reference to all meshes currently stored
		@param textures - A reference to all textures currently stored
	*/
	void drawRig(const std::map<std::string, Graphics::mesh> &meshes, const std::map<std::string, uint16_t> &textures);

	/*
	Retrieves the node whose name matches the given string

	@param node_name - The given name

	Returns the reference to the Node that has the given name
	*/
	Node& getNode(std::string node_name);

	void addRotation(Math::vec3d rot) { this->rot += rot; }

	void setRotation(Math::vec3d rot) { this->rot = rot; }
	void setScale(Math::vec3d scale) { this->scale = scale; }


private:

	/*
		Rigs the model of a female elf
	*/
	void rigFemaleElf();

	/*
		Rigs the model of a goblin
	*/
	void rigGoblin();

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