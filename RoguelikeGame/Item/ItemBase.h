#pragma once

#include <Core/CraftObject.h>
#include <string>

/* 플레이어가 소유하는 아이템들 */
class ItemBase : public Craft::CraftObject
{
	TYPE_DECLARATIONS(ItemBase, CraftObject)

public:
	ItemBase(int id, int num);
	virtual ~ItemBase();

public:
	void SetItemNum(int num);

public:
	inline int GetItemID() const { return itemID; }
	inline int GetItemNum() const { return itemNum; }

private:
	/* 아이템 ID */
	int itemID = 0;

	/* 아이템 갯수 */
	int itemNum = 0;
};

