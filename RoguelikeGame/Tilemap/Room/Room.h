#pragma once

#include "Types/Enums.h"
#include <Math/Vector2.h>
#include <vector>
#include <functional>

class Room
{
public:
	using RoomTileIndices = std::vector<Craft::Vector2>;

public:
	Room(const Craft::Vector2& inPositionLT, int inWidth, int inHeight);
	virtual ~Room();

public:
	void InitializeRoom();
	void Foreach_Tile(std::function<void(const Craft::Vector2&)> CallbackFunc) const;

public:
	//외곽 타일중 랜덤하게 하나를 선택해서 문타일로 지정하고 반환함
	Craft::Vector2 SelectDoorTile(eRoomSides edge);

public:
	inline const Craft::Vector2& GetPositionCenter() const { return positionCenter; }


private:
	//방의 시작 좌표(좌상단)
	Craft::Vector2 positionLT = Craft::Vector2::Zero;

	//방의 Width
	int Width = 0;

	//방의 Height
	int Height = 0;

	//방의 중심 좌표
	Craft::Vector2 positionCenter = Craft::Vector2::Zero;

	//방 안의 타일 인덱스들
	RoomTileIndices tileIndices;

	//방 주변의 타일들 (4면 따로 저장) - 입구 구성에 사용
	RoomTileIndices outerTileIndices[static_cast<int>(eRoomSides::MaxNum)];

	//방의 문 타일 인덱스들
	RoomTileIndices doorTileIndices;
};

