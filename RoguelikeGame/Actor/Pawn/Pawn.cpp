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
	attributeComponent->SetOutofHealthCallback(std::bind(&Pawn::OnOutOfHealth, this));
	attributeComponent->SetChangeHealthValueCallback(std::bind(&Pawn::OnChangeHealthValue, this, std::placeholders::_1, std::placeholders::_2));

	abilitySystemComponent = AddComponent<AbilitySystemComponent>();
	assert(abilitySystemComponent && "Invalid AbilitySystemComponent");
}

void Pawn::BeginPlay()
{
	super::BeginPlay();

	InitializeAbility();
}

void Pawn::Destroy()
{
	if (abilitySystemComponent)
	{
		abilitySystemComponent->AllAbilitiesTriggerOff();
	}

	super::Destroy();
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

void Pawn::InitializeHealthValue(const float currentHealth, const float maxHealth)
{
	if (!attributeComponent)
	{
		return;
	}

	attributeComponent->InitializeHealthValue(currentHealth, maxHealth);
}

void Pawn::SetHealthChangeEventCallback(OnChangeHealthType callback)
{
	onChangeHealthEvent = callback;
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

void Pawn::AddHealthValue(const float inHealValue)
{
	if (!attributeComponent)
	{
		return;
	}

	attributeComponent->IncreaseCurrrentHealth(inHealValue);
}

bool Pawn::IsDeath() const
{
	if (!attributeComponent)
	{
		return false;
	}

	return attributeComponent->GetCurrentHealth() <= 0;
}

/* 원거리 공격 범위 지정 */
void Pawn::SetFireRange(float range)
{
	fireRange = range;
}

/* 투사체 Spawn Offset 지정 */
void Pawn::SetProjectileSpawnOffset(const Craft::Vector2Int& spawnOffset)
{
	projectileSpawnOffset = spawnOffset;
}

/* 조준 위치 지정 */
void Pawn::SetAimingPostion(const Craft::Vector2Int& position)
{
	aimingPosition = position;
}

/* 조준 방향 지정 */
void Pawn::SetAimingDirection(const Craft::Vector2Float& direction)
{
	aimingDirection = direction;
	aimingDirection.Normalize();
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

	/* 모든 Ability Trigger OFF */
	if (abilitySystemComponent)
	{
		abilitySystemComponent->AllAbilitiesTriggerOff();
	}
}

void Pawn::OnOutOfHealth()
{
	OnDeath();

	Destroy();
}

void Pawn::OnChangeHealthValue(float currentValue, float maxValue)
{
	if (onChangeHealthEvent)
	{
		onChangeHealthEvent(currentValue, maxValue);
	}
}
