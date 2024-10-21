#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace JonFolder {
	class Texture {
	public:
		Texture(const char* filePath, int filterMode, int wrapMode);
		~Texture();
		void Bind(unsigned int slot = 0);

	private:
		unsigned int textureID;
		int width, height;

	};
#endif
}