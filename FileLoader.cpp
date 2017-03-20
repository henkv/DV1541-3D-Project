#include "FileLoader.h"
#include "MtlFile.h"
#include "ObjFile.h"

FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

Model FileLoader::loadObj(std::string filePath)
{
	ObjFile obj(filePath);
	MtlFile mtl(obj.getMtlPath());
	return Model(obj.getMesh(), mtl.getMaterial());
}
