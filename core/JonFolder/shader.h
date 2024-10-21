#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <../ew/external/glad.h>"

#include <string>
#include <fstream>
#include <sstream>
#include<iostream>

namespace JonFolder {
	class ShaderClass {
	public:
		unsigned int compileShader(const char* vertexShaderSource, const char* fragmentShaderSource);

	};
}