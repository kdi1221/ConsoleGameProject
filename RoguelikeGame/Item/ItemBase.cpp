#include "ItemBase.h"

ItemBase::ItemBase(int id, int num)
	:itemID(id)
	,itemNum(num)
{
}

ItemBase::~ItemBase()
{

}

void ItemBase::SetItemNum(int num)
{
	itemNum = num;
}
