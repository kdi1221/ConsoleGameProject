#pragma once

#include "Math/Vector2.h"
#include "Types/Enums.h"

//타일맵 개별 타일 클래스
class Tile
{
public:
	Tile(eTileCategory InCategory, const Craft::Vector2& InPosition, size_t InTileIndex);
	virtual ~Tile();

public:
	//Getter
	inline eTileCategory GetTileCategory() const { return category; }
	inline const Craft::Vector2& GetTilePosition() const { return position; }
	inline size_t GetTileIndex() const { return index; }

	//Setter
	inline void SetTileCategory(eTileCategory InCategory) { category = InCategory; }

private:
	//타일 종류
	eTileCategory category = eTileCategory::None;

	//타일 위치(2차원 좌표 x, y)
	Craft::Vector2 position = Craft::Vector2::Zero;

	//타일 인덱스(배열 상 고유 인덱스)
	size_t index = -1;
};

