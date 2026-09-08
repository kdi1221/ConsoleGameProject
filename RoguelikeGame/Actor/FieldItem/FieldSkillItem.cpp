#include "FieldSkillItem.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

FieldSkillItem::FieldSkillItem(const Vector2Int& position, int inAbilityID, Craft::Color inColor)
	:super(position, L"A", inColor)
	,abilityID(inAbilityID)
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

		collidePlayerPawn->GainAbilityItem(*this);

		Destroy();

		Engine::Get().PlayOneShot("Effect/itemget.wav");
	}
}