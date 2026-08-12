#pragma once

#include "Actor/ActorOnTile.h"
#include "Math/Color.h"

//맵 상에 배치되는 오브젝트들의 부모 오브젝트
class MapObject : public ActorOnTile
{
	TYPE_DECLARATIONS(MapObject, ActorOnTile)

public:
	MapObject(const Craft::Vector2Int& position,
			const std::wstring& image,
			Craft::Color color);
	~MapObject() = default;

public:
	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor) override;
};

