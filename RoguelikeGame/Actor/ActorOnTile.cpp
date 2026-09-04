#include "ActorOnTile.h"
#include <Level/TilemapLevel.h>
#include <cassert>

using namespace Craft;

ActorOnTile::ActorOnTile(const Vector2Int& position)
	:super(position)
{

}

void ActorOnTile::OnTileOverlap(const eTileCategory tileCategory)
{

}

void ActorOnTile::ForEachOccupiedTileOffset(std::function<void(const Vector2Int&)> callbackFunc) const
{
	if (!callbackFunc)
	{
		return;
	}

	//기본적으로는 월드좌표를 기준 하나만 차지한다.
	return callbackFunc(Vector2Int::Zero);
}