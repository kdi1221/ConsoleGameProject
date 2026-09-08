#pragma once

#include "Actor/FieldItem/FieldItem.h"

class AttributeCrystal : public FieldItem
{
	TYPE_DECLARATIONS(AttributeCrystal, FieldItem)

public:
	enum class eAttributeCategory
	{
		None = 0,
		Health,
		Mana
	};

public:
	AttributeCrystal(const Craft::Vector2Int& position, eAttributeCategory category, const float amount);
	virtual ~AttributeCrystal();

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	eAttributeCategory attributeCategory = eAttributeCategory::None;
	float addAmount = 0.f;
};

