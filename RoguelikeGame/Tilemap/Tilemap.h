#pragma once

#include "Types/Defines.h"
#include "Math/Vector2Int.h"
#include <memory>
#include <vector>
#include <functional>

namespace Craft
{
	class Level;
}

class Tile;
class BSPNode;
class RoomSpace;
enum class eTileCategory;

//전체 Tile들을 관리하는 Tilemap 클래스
class Tilemap
{
public:
	using TileListType = std::vector<std::unique_ptr<Tile>>;

public:
	Tilemap(Craft::Level& inOwnerLevel);
	virtual ~Tilemap();

public:
	void InitializeTilemap(const Craft::Vector2Int& inMapSize, 
							std::function<void(const std::vector<Craft::Vector2Int>&)> CorridorCallback,
							std::function<void(std::unique_ptr<RoomSpace>)> RoomCallback);
	void Tick(float deltaTime);
	void Draw();

	void SetTileRoomIndex(int xPos, int yPos, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	void SetTileCategory(int xPos, int yPos, eTileCategory category);

	/* 해당 위치에 존재하는 타일 종류 반환 */
	eTileCategory GetTileCategory(const Craft::Vector2Int& position) const;
	RoomDefines::UNIQUE_INDEX_TYPE GetTileRoomIndex(const Craft::Vector2Int& position) const;

public:
	inline Craft::Level& GetOwnerLevel() const { return ownerLevel; }

private:
	Tile* GetTile(int xPos, int yPos) const;

private:
	//이 타일맵을 소유하는 Level;
	Craft::Level& ownerLevel;

	//타일맵 크기(가로 X 세로)
	Craft::Vector2Int mapSize;

	//타일맵 내의 타일들
	TileListType tileList;
};

