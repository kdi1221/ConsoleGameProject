#include "Pawn.h"
#include "Types/Enums.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include "Component/AttributeComponent.h"
#include "Actor/MapObject/RoomDoor.h"
#include <cassert>

using namespace Craft;

Pawn::Pawn(const Craft::Vector2Int& position,
			const std::wstring& image,
			Craft::Color color,
			int CollisionWidth,
			int initialHealth,
			eTeamID inTeamID)
	:super(position)
	,teamID(inTeamID)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::Pawn));
	AddComponent<BoxCollisionComponent>(CollisionWidth);

	attributeComponent = AddComponent<AttributeComponent>(initialHealth);
	assert(attributeComponent && "Invalid attributeComponent");
	attributeComponent->AddOutofHealthCallback(std::bind(&Pawn::OnOutOfHealth, this));
}

bool Pawn::IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor)
{
	assert(otherActor && "Invalid otherActor");

	/* Pawn끼리는 Block */
	if (otherActor->IsTypeOf<Pawn>())
	{
		return true;
	}

	/* 문 Actor와는 Block */
	if (otherActor->IsTypeOf<RoomDoor>())
	{
		return true;
	}

	return false;
}

void Pawn::SetDeathEventCallback(OnDeathEventType deathEventCallback)
{
	onDeathEvent = deathEventCallback;
}

void Pawn::TakeDamage(const int inDamage)
{
	if (!attributeComponent)
	{
		return;
	}

	attributeComponent->DecreaseCurrrentHealth(inDamage);
}

bool Pawn::IsDeath() const
{
	if (!attributeComponent)
	{
		return false;
	}

	return attributeComponent->GetCurrentHealth() <= 0;
}

void Pawn::OnDeath()
{
	if (onDeathEvent)
	{
		onDeathEvent(std::static_pointer_cast<Pawn>(shared_from_this()));
	}
}

void Pawn::OnOutOfHealth()
{
	OnDeath();

	Destroy();
}
