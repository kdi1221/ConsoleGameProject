#include "HealthPotion.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

HealthPotion::HealthPotion(const Craft::Vector2Int& position, const float heal)
	:super(position, L"P", Color::LightGreen)
	,healValue(heal)
{

}

HealthPotion::~HealthPotion()
{

}

void HealthPotion::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	if (other->IsTypeOf<PlayerPawn>())
	{
		/* 플레이어가 접근했으면 힐하고 삭제 */
		std::shared_ptr<PlayerPawn> collidePlayerPawn = Cast<PlayerPawn>(other);
		assert(collidePlayerPawn && "collidePlayerPawn invalid..");

		collidePlayerPawn->AddHealthValue(healValue);

		Destroy();

		Engine::Get().PlayOneShot("Effect/heal.wav");
	}
}
