#include "NPCMeleeBase.h"
#include "Ability/NPCAbility/Melee/AbilityMelee.h"
#include "Component/AbilitySystemComponent.h"
#include "Engine/Engine.h"
#include "Navigation/NavigationTilemap.h"
#include <cassert>

using namespace Craft;

NPCMeleeBase::NPCMeleeBase(const Craft::Vector2Int& position,
							const std::wstring& image,
							Craft::Color color,
							float initialHealth,
							float moveSpeed,
							float damageValue,
							float attackDuration,
							float attackFrameTime,
							RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, image, color, initialHealth, moveSpeed, eMonsterPattern::Melee, roomIndex)
	,damageAmount(damageValue)
	,attackDuration(attackDuration)
	,attackFrameTime(attackFrameTime)
{
	
}

void NPCMeleeBase::InitializeAbility()
{
	/* 근접 공격 Ability 부여 */
	grantedAttackAbilityID = abilitySystemComponent->AddNewAbility(1000, 1);
	AbilityMelee* grantedAttackAbility = abilitySystemComponent->GetAbility<AbilityMelee>(grantedAttackAbilityID);
	assert(grantedAttackAbility && "Invalid grantedAttackAbility");
	grantedAttackAbility->SetAttackFrame(attackDuration, attackFrameTime);
}

void NPCMeleeBase::ExecuteAttack()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponentPtr = GetAbilitySystemComponent();
	assert(abilitySystemComponentPtr && "Invalid abilitySystemComponent");

	abilitySystemComponentPtr->ActivateAbility(grantedAttackAbilityID);
}

void NPCMeleeBase::OnNotifyAttackFrame(const AbilityObject& ability)
{
	std::shared_ptr<Pawn> chaseTarget = GetChaseTarget();
	if (!chaseTarget || chaseTarget->IsDeath())
	{
		return;
	}

	const Vector2Int distance = chaseTarget->GetWorldPosition() - GetWorldPosition();

	/* 타겟이 사정거리 안에 있을때만(8방향) 공격 가능 */
	if (abs(distance.x) <= 1 && abs(distance.y) <= 1)
	{
		chaseTarget->TakeDamage(damageAmount);
	}
}