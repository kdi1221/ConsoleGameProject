#include "Config.h"
#include <cassert>

Config::Config()
{
	AddLoadConfigCallback("tilemapWidth", [this](const std::string& inKey, const std::string& inValue)
		{
			tilemapWidth = static_cast<int>(atoi(inValue.c_str()));
			assert(tilemapWidth > 0);
			return tilemapWidth > 0;
		});

	AddLoadConfigCallback("tilemapHeight", [this](const std::string& inKey, const std::string& inValue)
		{
			tilemapHeight = static_cast<int>(atoi(inValue.c_str()));
			assert(tilemapHeight > 0);
			return tilemapHeight > 0;
		});

	AddLoadConfigCallback("bspMinRoomWidth", [this](const std::string& inKey, const std::string& inValue)
		{
			bspMinRoomWidth = static_cast<int>(atoi(inValue.c_str()));
			assert(bspMinRoomWidth >= 10);
			return bspMinRoomWidth >= 10;
		});

	AddLoadConfigCallback("bspMinRoomHeight", [this](const std::string& inKey, const std::string& inValue)
		{
			bspMinRoomHeight = static_cast<int>(atoi(inValue.c_str()));
			assert(bspMinRoomHeight >= 10);
			return bspMinRoomHeight >= 10;
		});

	AddLoadConfigCallback("bspRoomWallThickness", [this](const std::string& inKey, const std::string& inValue)
		{
			bspRoomWallThickness = static_cast<int>(atoi(inValue.c_str()));
			assert(bspRoomWallThickness >= 2);
			return bspRoomWallThickness >= 2;
		});
}
