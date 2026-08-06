#pragma once

#include "Types/Enums.h"
#include <Math/Vector2.h>
#include <vector>
#include <functional>

class RoomSpace
{
public:
	using RoomTileIndices = std::vector<Craft::Vector2>;

public:
	RoomSpace(const Craft::Vector2& inPositionLT, int inWidth, int inHeight);
	virtual ~RoomSpace();

public:
	//방 공간 초기화
	void InitializeRoomSpace();

	//외곽 타일중 랜덤하게 하나를 선택해서 문타일로 지정하고 반환함
	Craft::Vector2 SelectDoorTile(eRoomSides edge);

public:
	inline const Craft::Vector2& GetPositionCenter() const { return positionCenter; }
	inline const RoomTileIndices& GetInnerTileIndices() const { return innerTileIndices; }
	inline const RoomTileIndices& GetOuterTileIndices(eRoomSides edge) const { return outerTileIndices[static_cast<int>(edge)]; }

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
	RoomTileIndices innerTileIndices;

	//방 주변의 타일들 (4면 따로 저장) - 입구 구성에 사용
	RoomTileIndices outerTileIndices[static_cast<int>(eRoomSides::MaxNum)];

	//방의 문 타일 인덱스들
	RoomTileIndices doorTileIndices;
};

