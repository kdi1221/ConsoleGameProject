#include "NPCMeleeBase.h"
#include "Ability/Melee/AbilityMelee.h"
#include "Component/AbilitySystemComponent.h"
#include "Engine/Engine.h"
#include "Navigation/NavigationTilemap.h"
#include <cassert>

using namespace Craft;

NPCMeleeBase::NPCMeleeBase(const Craft::Vector2Int& position,
							const std::wstring& image,
							Craft::Color color,
							float initialHealth,
							RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
							float moveDelay,
							float chaseDelay,
							float attackDelay,
							float damageValue)
	:super(position, image, color, initialHealth, roomIndex, moveDelay, chaseDelay)
	,attackDelayInterval(attackDelay)
	,attackDamage(damageValue)
{
	
}

void NPCMeleeBase::InitializeAbility()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	/* 근접 공격 Ability 추가 */
	grantMeleeAttackID = abilitySystemComponent->AddNewAbility<AbilityMelee>(attackDelayInterval, attackDamage);
}

void NPCMeleeBase::GetAvailableChaseTargetPosition(const Vector2Int& targetPos,
													std::vector<Vector2Int>& availablePosition)
{
	const NavigationTilemap& navigationSystem = Engine::Get().GetNavigationSystem<NavigationTilemap>();

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (y == 0 && x == 0)
			{
				continue;
			}

			const Vector2Int checkPos(targetPos + Vector2Int(x, y));
			if (!navigationSystem.CanNextMove(shared_from_this(), checkPos))
			{
				continue;
			}

			availablePosition.emplace_back(checkPos);
		}
	}
}

bool NPCMeleeBase::IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const
{
	if (!targetPawn)
	{
		return false;
	}

	const Vector2Int& targetPos = targetPawn->GetWorldPosition();
	const Vector2Int& worldPos = GetWorldPosition();
	const Vector2Int distance = targetPos - worldPos;

	return abs(distance.x) <= 1 && abs(distance.y) <= 1;
}

void NPCMeleeBase::AttackAbilitiesTriggerON()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	AbilityMelee* attackAbility = abilitySystemComponent->GetAbility<AbilityMelee>(grantMeleeAttackID);
	if (!attackAbility)
	{
		return;
	}

	attackAbility->SetTargetPawn(GetChaseTarget());
	attackAbility->TriggerOn();
}

void NPCMeleeBase::AttackAbilitiesTriggerOFF()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	AbilityMelee* attackAbility = abilitySystemComponent->GetAbility<AbilityMelee>(grantMeleeAttackID);
	if (!attackAbility)
	{
		return;
	}

	attackAbility->TriggerOff();
}
