#pragma once

#include "Actor/FieldItem/FieldItem.h"

/* 마나 포션 */
class ManaPotion : public FieldItem
{
	TYPE_DECLARATIONS(ManaPotion, FieldItem)

public:
	ManaPotion(const Craft::Vector2Int& position, const float heal);
	virtual ~ManaPotion();

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	float healValue = 0.f;
};