#include <fstream>
#include <strstream>
#include <sstream>
#include <iostream>
#include <string>

#include "ObjLoader.h"

static std::vector<std::string> split(std::string str, const char separator)
{
	std::vector<std::string> strings;
	std::istringstream f(str);
	std::string s;

	while (std::getline(f, s, separator)) {
		strings.push_back(s);
	}

	return strings;
}

Graphics::mesh ObjLoader::loadObj(std::string filename)
{
	Graphics::mesh mesh;

	std::ifstream f(filename);
	if (!f.is_open())
		return mesh;

	std::vector<Math::vec3d> verts;
	std::vector<Math::vec2d> texture_coords;
	std::vector<Math::vec3d> normals;

	while (!f.eof())
	{
		std::string line;
		std::getline(f, line);
		std::strstream s;
		s << line;

		char junk;

		//Vertex
		if (line[0] == 'v' && line[1] == ' ')
		{
			Math::vec3d v;
			s >> junk >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}
		//Texture
		else if (line[0] == 'v' && line[1] == 't') {
			Math::vec2d vt;
			s >> junk >> junk >> vt.x >> vt.y;
			texture_coords.push_back(vt);
		}
		//Normal
		else if (line[0] == 'v' && line[1] == 'n') {
			Math::vec3d vn;
			s >> junk >> junk >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		//Polygon
		else if (line[0] == 'f')
		{
			std::string remaining = line;
			remaining = remaining.substr(2, remaining.size());
			std::vector<std::string> indices = split(remaining, ' ');

			std::vector<std::vector<std::string>> verticesdata;
			for (int i = 0; i < indices.size(); i++) {
				std::vector<std::string> vertexdata = split(indices[i], '/');

				verticesdata.push_back(vertexdata);

				if (i >= 2) {
					Graphics::triangle tri;

					if (texture_coords.size() > 0) {
						tri.vt[0] = texture_coords[atoi(verticesdata[0][1].c_str()) - 1];
						tri.vt[1] = texture_coords[atoi(verticesdata[i - 1][1].c_str()) - 1];
						tri.vt[2] = texture_coords[atoi(verticesdata[i][1].c_str()) - 1];
					}

					if (normals.size() > 0) {
						tri.vn[0] = normals[atoi(verticesdata[0][2].c_str()) - 1];
						tri.vn[1] = normals[atoi(verticesdata[i - 1][2].c_str()) - 1];
						tri.vn[2] = normals[atoi(verticesdata[i][2].c_str()) - 1];
					}
					
					tri.p[0] = verts[atoi(verticesdata[0][0].c_str()) - 1];
					tri.p[1] = verts[atoi(verticesdata[i - 1][0].c_str()) - 1];					
					tri.p[2] = verts[atoi(verticesdata[i][0].c_str()) - 1];
					
					tri.fn = Graphics::triangle_getNormal(tri);

					mesh.tris.push_back(tri);
				}
			}

		}
	}

	return mesh;
}
