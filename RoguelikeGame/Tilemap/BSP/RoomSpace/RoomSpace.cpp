#include "RoomSpace.h"
#include <Util/Util.h>
#include <cassert>

using namespace Craft;

RoomSpace::RoomSpace(const Craft::Vector2Int& inPositionLT, int inRoomWidth, int inRoomHeight,
						const Craft::Vector2Int& inOuterPositionLT, int inOuterWidth, int inOuterHeight)
	:positionLT(inPositionLT)
	,Width(inRoomWidth)
	,Height(inRoomHeight)
	,outerPositionLT(inOuterPositionLT)
	,outerWidth(inOuterWidth)
	,outerHeight(inOuterHeight)
{

}

RoomSpace::~RoomSpace()
{

}

void RoomSpace::InitializeRoomSpace()
{
	//positionCenter 계산
	positionCenter = positionLT + Vector2Int(Width >> 1, Height >> 1);

	//tileIndices 계산
	innerTileIndices.reserve(Width * Height);
	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			const int XPos = positionLT.x + x;
			const int YPos = positionLT.y + y;
			innerTileIndices.emplace_back(Vector2Int(XPos, YPos));
		}
	}

	//방 주변 문 후보 타일들 인덱스 계산
	const int leftXPos = positionLT.x - 1;
	const int leftYPos = positionLT.y + 1;
	for (int addYPos = 0; addYPos < Height - 2; ++addYPos)
	{
		const int xPos = leftXPos;
		const int yPos = leftYPos + addYPos;

		outerTileIndices[static_cast<int>(eRoomSides::Left)].emplace_back(Vector2Int(xPos, yPos));
	}

	const int topXPos = positionLT.x + 1;
	const int topYPos = positionLT.y - 1;
	for (int addXPos = 0; addXPos < Width - 2; ++addXPos)
	{
		const int xPos = topXPos + addXPos;
		const int yPos = topYPos;

		outerTileIndices[static_cast<int>(eRoomSides::Top)].emplace_back(Vector2Int(xPos, yPos));
	}

	const int rightXPos = positionLT.x + Width;
	const int rightYPos = positionLT.y + 1;
	for (int addYPos = 0; addYPos < Height - 2; ++addYPos)
	{
		const int xPos = rightXPos;
		const int yPos = rightYPos + addYPos;

		outerTileIndices[static_cast<int>(eRoomSides::Right)].emplace_back(Vector2Int(xPos, yPos));
	}

	const int bottomXPos = positionLT.x + 1;
	const int bottomYPos = positionLT.y + Height;
	for (int addXPos = 0; addXPos < Width - 2; ++addXPos)
	{
		const int xPos = bottomXPos + addXPos;
		const int yPos = bottomYPos;

		outerTileIndices[static_cast<int>(eRoomSides::Bottom)].emplace_back(Vector2Int(xPos, yPos));
	}
}

Craft::Vector2Int RoomSpace::SelectDoorTile(eRoomSides edge)
{
	std::vector<Vector2Int>& edgeOuterTileIndices = outerTileIndices[static_cast<int>(edge)];

	assert(!edgeOuterTileIndices.empty() && "edgeOuterTileIndices empty..");

	const int selectTileIndex = Util::RandomRange(0, static_cast<int>(edgeOuterTileIndices.size()) - 1);
	Vector2Int selectTile = edgeOuterTileIndices[selectTileIndex];

	//선택된 외곽 타일 인덱스 제거
	edgeOuterTileIndices.erase(edgeOuterTileIndices.begin() + selectTileIndex);

	//문 타일로 저장한다.
	doorTileIndices.emplace_back(selectTile);

	return selectTile;
}
