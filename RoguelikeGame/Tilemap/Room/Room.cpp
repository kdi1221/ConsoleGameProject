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
}

void Room::Foreach_Tile(std::function<void(const Craft::Vector2&)> CallbackFunc) const
{
	for (const auto& tileIndex : tileIndices)
	{
		CallbackFunc(tileIndex);
	}
}