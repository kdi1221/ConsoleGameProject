#pragma once

#include "Engine/Config/ConfigBase.h"

class Config : public Craft::ConfigBase
{
public:
	Config();
	~Config() = default;

public:
	inline int GetTilemapWidth() const { return tilemapWidth; }
	inline int GetTilemapHeight() const { return tilemapHeight; }
	inline int GetBSPMinRoomLength() const { return bspMinRoomLength; }
	inline int GetBSPRoomWallThickness() const { return bspRoomWallThickness; }

private:
	//타일맵 가로 길이
	int tilemapWidth = 0;

	//타일맵 세로 길이
	int tilemapHeight = 0;

	//BSP 방 최소 길이(최소 10 이상)
	int bspMinRoomLength = 0;

	//BSP 벽 길이(최소 2이상)
	int bspRoomWallThickness = 0;
};


