#pragma once

#include <Actor/Actor.h>
#include <Types/Enums.h>

/* Tile 기반 위에서 움직이는 모든 Actor들의 부모 */
class ActorOnTile : public Craft::Actor
{
	TYPE_DECLARATIONS(ActorOnTile, Actor)

public:
	ActorOnTile(const Craft::Vector2Float& position = Craft::Vector2Float::Zero);
	virtual ~ActorOnTile() = default;

public:
	/* 특정 타일 위에 있는 동안 호출되는 함수 */
	virtual void OnTileOverlap(const eTileCategory tileCategory);
};

