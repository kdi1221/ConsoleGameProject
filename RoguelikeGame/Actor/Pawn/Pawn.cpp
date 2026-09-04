#include "Pawn.h"
#include "Types/Enums.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/Attribute/AttributeComponent.h"
#include "Component/AbilitySystemComponent.h"
#include "Actor/MapObject/RoomDoor.h"
#include "Actor/Projectile/Projectile.h"
#include "Util/Util.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

PawnUniqueIDType Pawn::GenerateUnqueID()
{
	static PawnUniqueIDType currentID = 1;

	PawnUniqueIDType allocUniqueID = currentID;

	++currentID;
	if (0 == currentID)
	{
		/* 계속 증가하다 순환되어 0이되면 1로 설정 */
		currentID = 1;
	}

	return allocUniqueID;
}

Pawn::Pawn(const Craft::Vector2Int& position,
			const std::wstring& image,
			Craft::Color color,
			eTeamID inTeamID)
	:super(position)
	,teamID(inTeamID)
{
	uniqueID = GenerateUnqueID();

	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::Pawn));
	
	abilitySystemComponent = AddComponent<AbilitySystemComponent>();
	assert(abilitySystemComponent && "Invalid AbilitySystemComponent");
}

Pawn::Pawn(const Craft::Vector2Int& position, 
			eTeamID inTeamID)
	:super(position)
	,teamID(inTeamID)
{
	uniqueID = GenerateUnqueID();

	abilitySystemComponent = AddComponent<AbilitySystemComponent>();
	assert(abilitySystemComponent && "Invalid AbilitySystemComponent");
}

void Pawn::Initialize()
{
	/* Pawn마다 다른 attribute component 생성 */
	attributeComponent = CreateAttributeComponent();
	assert(attributeComponent && "Invalid attributeComponent");

	/* Health Value 변화에 따른 콜백 설정 */
	attributeComponent->SetChangeHealthEventCallback(std::bind(&Pawn::OnChangeHealthValue, this, std::placeholders::_1, std::placeholders::_2));
}

void Pawn::BeginPlay()
{
	super::BeginPlay();

	InitializeAbility();
}

void Pawn::Destroy()
{
	/* Destory과정에서 모든 Ability 중단 */
	if (abilitySystemComponent)
	{
		abilitySystemComponent->CancelAllAbility();
	}

	super::Destroy();
}

bool Pawn::IsBlockActor(std::shared_ptr<Actor> otherActor)
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

	if (otherActor->IsTypeOf<Projectile>())
	{
		/* Projectile와는 다른팀일때 Block */
		std::shared_ptr<Projectile> blockProjectile = Cast<Projectile>(otherActor);
		if (blockProjectile && blockProjectile->GetInstigatorTeamID() != GetTeamID())
		{
			return true;
		}
	}

	return false;
}

void Pawn::InitializeHealthValue(const float currentHealth, const float maxHealth)
{
	assert(attributeComponent && "Invalid attributeComponent");

	/* 초기 속성 값 설정 */
	attributeComponent->InitialHealth(currentHealth, maxHealth);

	/* 초기 사망 설정 */
	bDeath = attributeComponent->GetCurrentHealth() <= 0.f;
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

	attributeComponent->SetCurrentHealth(attributeComponent->GetCurrentHealth() - inDamage);

	int randomHitSoundIndex = Util::RandomRange(1, 3);
	std::string hitSoundName = "Effect/hit" + std::to_string(randomHitSoundIndex) + ".wav";
	Engine::Get().PlayOneShot(hitSoundName);
}

void Pawn::AddHealthValue(const float inHealValue)
{
	if (!attributeComponent)
	{
		return;
	}

	attributeComponent->SetCurrentHealth(attributeComponent->GetCurrentHealth() + inHealValue);
}

bool Pawn::IsDeath() const
{
	return bDeath;
}

std::shared_ptr<AttributeComponent> Pawn::CreateAttributeComponent()
{
	return AddComponent<AttributeComponent>();
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
	/* 모든 Ability 활성화 중지 */
	if (abilitySystemComponent)
	{
		abilitySystemComponent->CancelAllAbility();
	}

	if (onDeathEvent)
	{
		onDeathEvent(std::static_pointer_cast<Pawn>(shared_from_this()));
	}

	Engine::Get().PlayOneShot("Effect/death.wav");
}

void Pawn::SetDeath()
{
	bDeath = true;

	OnDeath();

	Destroy();
}

void Pawn::OnChangeHealthValue(float currentValue, float maxValue)
{
	if (currentValue > 0.f)
	{
		if (onChangeHealthEvent)
		{
			onChangeHealthEvent(currentValue, maxValue);
		}
	}
	else
	{
		SetDeath();
	}
}
