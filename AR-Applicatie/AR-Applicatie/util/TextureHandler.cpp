#include <GL/glut.h>
#include <vector>

#include "TextureHandler.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static std::vector<uint16_t> texture_ids;

uint16_t TextureHandler::addTexture(const std::string image_path)
{
	GLuint texture_id = texture_ids.size();

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	texture_ids.push_back(texture_id);


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 4);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);

	stbi_image_free(data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture_id;
}
