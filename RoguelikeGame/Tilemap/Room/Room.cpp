#include "Room.h"

using namespace Craft;

Room::Room(const Craft::Vector2& inPositionLT, int inWidth, int inHeight)
	:positionLT(inPositionLT)
	,Width(inWidth)
	,Height(inHeight)
{

}

Room::~Room()
{

}

void Room::InitializeRoom()
{
	//positionCenter 계산
	positionCenter = positionLT + Vector2(Width >> 1, Height >> 1);

	//tileIndices 계산
	tileIndices.reserve(Width * Height);
	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			const int XPos = positionLT.x + x;
			const int YPos = positionLT.y + y;
			tileIndices.emplace_back(Vector2(XPos, YPos));
		}
	}

	//방 주변 문 후보 타일들 인덱스 계산
	const int leftXPos = positionLT.x - 1;
	const int leftYPos = positionLT.y;
	for (int addYPos = 0; addYPos < Height; ++addYPos)
	{
		const int xPos = leftXPos;
		const int yPos = leftYPos + addYPos;

		doorCandidateTiles[eRoomEdges::Left].emplace_back(Vector2(xPos, yPos));
	}

	const int topXPos = positionLT.x;
	const int topYPos = positionLT.y - 1;
	for (int addXPos = 0; addXPos < Width; ++addXPos)
	{
		const int xPos = topXPos + addXPos;
		const int yPos = topYPos;

		doorCandidateTiles[eRoomEdges::Top].emplace_back(Vector2(xPos, yPos));
	}

	const int rightXPos = positionLT.x + Width;
	const int rightYPos = positionLT.y;
	for (int addYPos = 0; addYPos < Height; ++addYPos)
	{
		const int xPos = rightXPos;
		const int yPos = rightYPos + addYPos;

		doorCandidateTiles[eRoomEdges::Right].emplace_back(Vector2(xPos, yPos));
	}

	const int bottomXPos = positionLT.x;
	const int bottomYPos = positionLT.y + Height;
	for (int addXPos = 0; addXPos < Width; ++addXPos)
	{
		const int xPos = bottomXPos + addXPos;
		const int yPos = bottomYPos;

		doorCandidateTiles[eRoomEdges::Bottom].emplace_back(Vector2(xPos, yPos));
	}
}

void Room::Foreach_Tile(std::function<void(const Craft::Vector2&)> CallbackFunc) const
{
	for (const auto& tileIndex : tileIndices)
	{
		CallbackFunc(tileIndex);
	}
}

const std::vector<Craft::Vector2>& Room::GetDoorCandidateTiles(eRoomEdges edge) const
{
	return doorCandidateTiles[edge];
}
