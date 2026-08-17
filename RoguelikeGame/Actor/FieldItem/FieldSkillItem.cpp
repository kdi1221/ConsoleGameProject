#include "FieldSkillItem.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include "Item/ItemData/ItemDataTable.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

FieldSkillItem::FieldSkillItem(const Vector2Int& position, int inItemID)
	:FieldSkillItem(position, inItemID, ItemDataTable::GetItemData(inItemID))
{
	
}

FieldSkillItem::FieldSkillItem(const Vector2Int& position, int inItemID, const ItemData& itemData)
	:super(position, itemData.fieldItemImage, itemData.color)
	,itemID(inItemID)
{

}

void FieldSkillItem::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	if (other->IsTypeOf<PlayerPawn>())
	{
		/* 플레이어가 접근했으면 아이템 추가하고 삭제 */
		std::shared_ptr<PlayerPawn> collidePlayerPawn = Cast<PlayerPawn>(other);
		assert(collidePlayerPawn && "collidePlayerPawn invalid..");

		collidePlayerPawn->GainSkillItem(std::static_pointer_cast<FieldSkillItem>(shared_from_this()));

		Destroy();

		Engine::Get().PlayOneShot("Effect/itemget.wav");
	}
}