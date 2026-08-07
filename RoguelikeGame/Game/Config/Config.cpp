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

	AddLoadConfigCallback("bspMinRoomLength", [this](const std::string& inKey, const std::string& inValue)
		{
			bspMinRoomLength = static_cast<int>(atoi(inValue.c_str()));
			assert(bspMinRoomLength >= 10);
			return bspMinRoomLength >= 10;
		});

	AddLoadConfigCallback("bspRoomWallThickness", [this](const std::string& inKey, const std::string& inValue)
		{
			bspRoomWallThickness = static_cast<int>(atoi(inValue.c_str()));
			assert(bspRoomWallThickness >= 2);
			return bspRoomWallThickness >= 2;
		});
}
