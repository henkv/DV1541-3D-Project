#pragma once
#include "Model.h"

class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	static Model loadObj(std::string filePath);
};

