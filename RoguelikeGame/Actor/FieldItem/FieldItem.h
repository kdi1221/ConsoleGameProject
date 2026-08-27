#pragma once

#include "Actor/ActorOnTile.h"

/* 필드에 존재하는 아이템 객체 */
class FieldItem : public ActorOnTile
{
	TYPE_DECLARATIONS(FieldItem, ActorOnTile)

public:
	FieldItem(const Craft::Vector2Int& position,
			const std::wstring& image,
			Craft::Color color);
	virtual ~FieldItem() = default;

public:
	/* 다른 종류의 Actor와 Block되는지 체크 */
	virtual bool IsBlockActor(std::shared_ptr<Actor> otherActor) override;
};

