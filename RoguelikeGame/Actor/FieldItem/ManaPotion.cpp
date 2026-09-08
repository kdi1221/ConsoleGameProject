#include "ManaPotion.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

ManaPotion::ManaPotion(const Craft::Vector2Int& position, const float heal)
	:super(position, L"P", Color::LightBlue)
	, healValue(heal)
{

}

ManaPotion::~ManaPotion()
{

}

void ManaPotion::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	if (other->IsTypeOf<PlayerPawn>())
	{
		/* 플레이어가 접근했으면 힐하고 삭제 */
		std::shared_ptr<PlayerPawn> collidePlayerPawn = Cast<PlayerPawn>(other);
		assert(collidePlayerPawn && "collidePlayerPawn invalid..");

		collidePlayerPawn->AddManaValue(healValue);

		Destroy();

		Engine::Get().PlayOneShot("Effect/heal.wav");
	}
}