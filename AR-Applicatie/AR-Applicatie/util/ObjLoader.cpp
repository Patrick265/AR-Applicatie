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

					Graphics::vertex vert;
					Graphics::vertex vert2;
					Graphics::vertex vert3;

					if (texture_coords.size() > 0)
					{
						vert.vt = texture_coords[atoi(verticesdata[0][1].c_str()) - 1];
						vert2.vt = texture_coords[atoi(verticesdata[i - 1][1].c_str()) - 1];
						vert3.vt = texture_coords[atoi(verticesdata[i][1].c_str()) - 1];
					}
					if (normals.size() > 0)
					{
						vert.vn = normals[atoi(verticesdata[0][2].c_str()) - 1];
						vert2.vn = normals[atoi(verticesdata[i - 1][2].c_str()) - 1];
						vert3.vn = normals[atoi(verticesdata[i][2].c_str()) - 1];
					}

					vert.p = verts[atoi(verticesdata[0][0].c_str()) - 1];
					vert2.p = verts[atoi(verticesdata[i - 1][0].c_str()) - 1];
					vert3.p = verts[atoi(verticesdata[i][0].c_str()) - 1];

					vert.fn = Graphics::getNormal(vert.p, vert2.p, vert3.p);
					vert2.fn = Graphics::getNormal(vert.p, vert2.p, vert3.p);
					vert3.fn = Graphics::getNormal(vert.p, vert2.p, vert3.p);

					mesh.vertices.push_back(vert);
					mesh.vertices.push_back(vert2);
					mesh.vertices.push_back(vert3);
				}
			}

		}
	}

	return mesh;
}
