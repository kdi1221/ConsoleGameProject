#pragma once

#include "Actor/FieldItem/FieldItem.h"

struct ItemData;

/* 필드 내 스킬 아이템 */
class FieldSkillItem : public FieldItem
{
	TYPE_DECLARATIONS(FieldSkillItem, FieldItem)

public:
	FieldSkillItem(const Craft::Vector2Int& position, int inItemID);

private:
	FieldSkillItem(const Craft::Vector2Int& position, int inItemID, const ItemData& itemData);

public:
	inline int GetItemID() const { return itemID; }

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	int itemID = 0;
};

