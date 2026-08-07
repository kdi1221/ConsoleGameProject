#pragma once

#include "Types/Enums.h"
#include <Math/Vector2Int.h>
#include <vector>
#include <functional>

class RoomSpace
{
public:
	using RoomTileIndices = std::vector<Craft::Vector2Int>;

public:
	RoomSpace(const Craft::Vector2Int& inPositionLT, int inRoomWidth, int inRoomHeight, 
				const Craft::Vector2Int& inOuterPositionLT, int inOuterWidth, int inOuterHeight);
	virtual ~RoomSpace();

public:
	//방 공간 초기화
	void InitializeRoomSpace();

	//외곽 타일중 랜덤하게 하나를 선택해서 문타일로 지정하고 반환함
	Craft::Vector2Int SelectDoorTile(eRoomSides edge);

public:
	inline const Craft::Vector2Int& GetPostionLT() const { return positionLT; }
	inline const Craft::Vector2Int& GetPositionCenter() const { return positionCenter; }
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
	inline const Craft::Vector2Int& GetOuterPostionLT() const { return outerPositionLT; }
	inline int GetOuterWidth() const { return outerWidth; }
	inline int GetOuterHeight() const { return outerHeight; }
	inline const RoomTileIndices& GetInnerTileIndices() const { return innerTileIndices; }
	inline const RoomTileIndices& GetOuterTileIndices(eRoomSides edge) const { return outerTileIndices[static_cast<int>(edge)]; }

private:
	//방의 시작 좌표(좌상단)
	Craft::Vector2Int positionLT = Craft::Vector2Int::Zero;

	//방의 Width
	int Width = 0;

	//방의 Height
	int Height = 0;

	//방의 중심 좌표
	Craft::Vector2Int positionCenter = Craft::Vector2Int::Zero;

	//방을 감싸는 외곽 영역의 시작 좌표(좌상단)
	Craft::Vector2Int outerPositionLT = Craft::Vector2Int::Zero;

	//방을 감싸는 외곽 영역의 Width
	int outerWidth = 0;

	//방을 감싸는 외곽 영역의 Height
	int outerHeight = 0;

	//방 안의 타일 인덱스들
	RoomTileIndices innerTileIndices;

	//방 주변의 타일들 (4면 따로 저장) - 입구 구성에 사용
	RoomTileIndices outerTileIndices[static_cast<int>(eRoomSides::MaxNum)];

	//방의 문 타일 인덱스들
	RoomTileIndices doorTileIndices;
};

