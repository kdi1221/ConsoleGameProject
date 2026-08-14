#include "Pawn.h"
#include "Types/Enums.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/AttributeComponent.h"
#include "Component/AbilitySystemComponent.h"
#include "Actor/MapObject/RoomDoor.h"
#include <cassert>

using namespace Craft;

Pawn::Pawn(const Craft::Vector2Int& position,
			const std::wstring& image,
			Craft::Color color,
			float initialHealth,
			eTeamID inTeamID)
	:super(position)
	,teamID(inTeamID)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::Pawn));

	attributeComponent = AddComponent<AttributeComponent>(initialHealth);
	assert(attributeComponent && "Invalid attributeComponent");
	attributeComponent->AddOutofHealthCallback(std::bind(&Pawn::OnOutOfHealth, this));

	abilitySystemComponent = AddComponent<AbilitySystemComponent>();
	assert(abilitySystemComponent && "Invalid AbilitySystemComponent");
}

void Pawn::BeginPlay()
{
	super::BeginPlay();

	InitializeAbility();
}

void Pawn::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (abilitySystemComponent)
	{
		abilitySystemComponent->Tick(deltaTime);
	}
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

void Pawn::TakeDamage(const float inDamage)
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

void Pawn::InitializeAbility()
{

}

void Pawn::OnDeath()
{
	if (onDeathEvent)
	{
		onDeathEvent(std::static_pointer_cast<Pawn>(shared_from_this()));
	}
}

void Pawn::AbilitiesTriggerOn()
{
	if (!abilitySystemComponent)
	{
		return;
	}

	abilitySystemComponent->AbilityTriggerOn();
}

void Pawn::AbilitiesTriggerOff()
{
	if (!abilitySystemComponent)
	{
		return;
	}

	abilitySystemComponent->AbilityTriggerOff();
}

void Pawn::OnOutOfHealth()
{
	OnDeath();

	Destroy();
}
