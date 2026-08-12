#pragma once

#include "Actor/MapObject/MapObject.h"

/* 방의 입구에서 생성되는 문 오브젝트 */
class RoomDoor : public MapObject
{
	TYPE_DECLARATIONS(RoomDoor, MapObject)

public:
	RoomDoor(const Craft::Vector2Int& position);
	virtual ~RoomDoor() = default;

public:
	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor) override;
};

