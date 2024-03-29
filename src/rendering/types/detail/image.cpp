#include <stb_image.h>
#include <glad/glad.h>
#include "core/log.h"
#include "../image.h"

Image::Image(std::string path, bool isRGBA)
{
	this->isRGBA = isRGBA;
	this-> path = path;

	// Load and generate the texture
	int nrChannels;
	data = (uint32_t*)stbi_load(path.c_str(), &w, &h, &nrChannels, 0);

	if (!data)
		Logger::getInstance().err("Failed to load texture " + path);
}

Image::Image(uint32_t* data, int width, int height, bool isRGBA)
{
	this->data = data;
	this->path = "img:" + std::to_string(rand());
	w = width;
	h = height;
	this->isRGBA = isRGBA;
}

bool Image::IsRGBA() { return isRGBA; }

// Blit a portion of this image onto `dest` at destX, destY (Both images have to be RGBA!)
void Image::Blit(Image* dest, size_t srcX, size_t srcY, size_t srcWidth, size_t srcHeight, size_t destX, size_t destY)
{
	if (!isRGBA || !dest->IsRGBA())
	{
		Logger::getInstance().err("Failed to blit Image " + path + " to Image " + dest->path + " as one or both Image(s) aren't RGBA");
		return;
	}

	if (srcX + srcWidth > w || srcY + srcHeight > h || destX + srcWidth > dest->w || destY + srcHeight > dest->h)
		Logger::getInstance().err("Failed to blit Image " + path + " to Image " + dest->path + " as the image bounds are out of range");


	for (size_t y = 0; y < srcHeight; y++)
		std::memcpy(&dest->data[(destY + y) * dest->w + destX], &data[(srcY + y) * w + srcX], srcWidth * sizeof(uint32_t));
}