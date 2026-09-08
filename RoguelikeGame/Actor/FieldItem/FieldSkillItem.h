#pragma once

#include "Actor/FieldItem/FieldItem.h"
#include <Math/Color.h>

struct ItemData;

/* 필드 내 스킬 아이템 */
class FieldSkillItem : public FieldItem
{
	TYPE_DECLARATIONS(FieldSkillItem, FieldItem)

public:
	FieldSkillItem(const Craft::Vector2Int& position, int inAbilityID, Craft::Color inColor);

public:
	inline int GetItemAbilityID() const { return abilityID; }

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	int abilityID = 0;
};

