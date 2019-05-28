#pragma once

#include "../calculations/Graphics.h"

class ObjLoader {
public:
	/*
		Loads the obj file and extracts the vertices which form the mesh

		@param filename - The name of the file

		Returns the extracted mesh
	*/
	static Graphics::mesh loadObj(const std::string filename);


private:
	/*
	Adds a triangle to the given mesh

	@param mesh           - The given mesh
	@param verts          - The vertices
	@param texture_coords - The texture coordinates
	@param normals        - The normals
	@param index		  - The index of the vertex
	@param verticesdata   - The values of all the vertices, texture coordinates and normals
	*/
	static void addTriangle(Graphics::mesh &mesh,
		const std::vector<Math::vec3d> &verts,
		const std::vector<Math::vec2d> &texture_coords,
		const std::vector<Math::vec3d> &normals,
		const int index,
		const std::vector<std::vector<std::string>> &verticesdata);

};