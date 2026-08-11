#pragma once

#include <Actor/Actor.h>

//class TilemapLevel;

/* Tile 기반 위에서 움직이는 모든 Actor들의 부모 */
class ActorOnTile : public Craft::Actor
{
	TYPE_DECLARATIONS(ActorOnTile, Actor)

public:
	ActorOnTile(const Craft::Vector2Float& position = Craft::Vector2Float::Zero);
	virtual ~ActorOnTile() = default;

//protected:
//	virtual void BeginPlay() override;
};

