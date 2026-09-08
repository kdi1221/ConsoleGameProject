#include "AttributeCrystal.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

AttributeCrystal::AttributeCrystal(const Craft::Vector2Int& position, eAttributeCategory category, const float amount)
	:super(position, L"C", category == eAttributeCategory::Health ? Color::LightRed : Color::LightBlue)
	,attributeCategory(category)
	,addAmount(amount)
{
}

AttributeCrystal::~AttributeCrystal()
{

}

void AttributeCrystal::OnCollision(const std::shared_ptr<Actor>&other)
{
	super::OnCollision(other);

	if (other->IsTypeOf<PlayerPawn>())
	{
		/* 플레이어가 접근했으면 Attribute 속성 증가하고 삭제 */
		std::shared_ptr<PlayerPawn> collidePlayerPawn = Cast<PlayerPawn>(other);
		assert(collidePlayerPawn && "collidePlayerPawn invalid..");

		switch (attributeCategory)
		{
		case eAttributeCategory::Health:
			collidePlayerPawn->IncreasesMaxHealth(addAmount);
			break;

		case eAttributeCategory::Mana:
			collidePlayerPawn->IncreasesMaxMana(addAmount);
			break;
		}

		Destroy();

		Engine::Get().PlayOneShot("Effect/heal.wav");
	}
}
