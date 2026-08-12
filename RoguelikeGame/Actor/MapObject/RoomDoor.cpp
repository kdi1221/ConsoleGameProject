#include "RoomDoor.h"
#include "Actor/Pawn/Pawn.h"
#include <cassert>

RoomDoor::RoomDoor(const Craft::Vector2Int& position)
	:super(position, L"▤", Craft::Color::Red)
{

}

bool RoomDoor::IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor)
{
	assert(otherActor && "Invalid otherActor");

	/* Pawn하고는 겹칠 수 없음*/
	if (otherActor->IsTypeOf<Pawn>())
	{
		return true;
	}

	/* 그 외 오브젝트와는 겹치기 가능 */
	return false;
}
