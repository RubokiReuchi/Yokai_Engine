#include "Resource.h"
#include"Globals.h"

Resource::Resource(int uid, Resource::Type type) : uid(uid), assetsFile(""), libraryFile(""), type(type),
referenceCount(0)
{

}

Resource::~Resource()
{
	uid = 0;
	assetsFile.clear();
	libraryFile.clear();
	type = Type::UNKNOWN;

	if (referenceCount != 0)
		LOG("DELETING RESOURCE WITH MULTIPLE REFERENCE COUNTS");

	referenceCount = 0;
}

void Resource::SetAssetsPath(const char* aPath)
{
	assetsFile = aPath;
}

void Resource::SetLibraryPath(const char* lPath)
{
	libraryFile = lPath;
}