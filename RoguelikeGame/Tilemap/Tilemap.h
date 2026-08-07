#pragma once

#include "Types/Defines.h"
#include "Math/Vector2Int.h"
#include <memory>
#include <vector>
#include <map>

class Tile;
class BSPNode;
class Room;
enum class eTileCategory;

using namespace RoomDefines;

//전체 Tile들을 관리하는 Tilemap 클래스
class Tilemap
{
public:
	Tilemap();
	virtual ~Tilemap();

public:
	void InitializeTilemap(const Craft::Vector2Int& inMapSize);
	void Tick(float deltaTime);
	void Draw();

	void SetTileCategory(int xPos, int yPos, eTileCategory category);

private:
	//타일맵 크기(가로 X 세로)
	Craft::Vector2Int mapSize;

	//타일맵 내의 타일들
	std::vector<std::unique_ptr<Tile>> tileList;

	//맵 내의 방 정보들
	std::map<RoomDefines::UNIQUE_INDEX_TYPE, std::unique_ptr<Room>> mapRooms;
};

