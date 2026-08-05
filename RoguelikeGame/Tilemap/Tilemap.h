#pragma once

#include "Math/Vector2.h"
#include <memory>
#include <vector>

class Tile;
enum class eTileCategory;
class BSPNode;

//전체 Tile들을 관리하는 Tilemap 클래스
class Tilemap
{
public:
	Tilemap();
	virtual ~Tilemap();

public:
	void InitializeTilemap(const Craft::Vector2& inMapSize);
	void Tick(float deltaTime);
	void Draw();

	void SetTileCategory(int xPos, int yPos, eTileCategory category);

//private:
	//void GenerateTiles();

private:
	//타일맵 크기(가로 X 세로)
	Craft::Vector2 mapSize;

	//타일맵 내의 타일들
	std::vector<std::unique_ptr<Tile>> TileList;

	//구성된 BSP 트리 정보
	std::unique_ptr<BSPNode> BSPRoot;
};

