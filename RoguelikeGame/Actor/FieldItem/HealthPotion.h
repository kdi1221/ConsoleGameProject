#pragma once

#include "Actor/FieldItem/FieldItem.h"

/* 체력 포션 */
class HealthPotion : public FieldItem
{
	TYPE_DECLARATIONS(HealthPotion, FieldItem)

public:
	HealthPotion(const Craft::Vector2Int& position, const float heal);
	virtual ~HealthPotion();

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	float healValue = 0.f;
};

