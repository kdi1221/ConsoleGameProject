#include "NPCRangeBase.h"
#include "Component/AbilitySystemComponent.h"
#include "Component/BehaviorTree/BehaviorRangeComponent.h"
#include "Ability/NPCAbility/Range/AbilityRange.h"
#include <cassert>

using namespace Craft;

NPCRangeBase::NPCRangeBase(const Vector2Int& position, 
							const std::wstring& image, 
							Color color, 
							float initialHealth, 
							float moveSpeed, 
							float attackDuration, 
							float attackFrameTime, 
							float enableFireRange,
							RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, image, color, initialHealth, moveSpeed, eMonsterPattern::Range, roomIndex)
	, attackDuration(attackDuration)
	, attackFrameTime(attackFrameTime)
{
	std::shared_ptr<BehaviorRangeComponent> behaviorRangeComponent = Cast<BehaviorRangeComponent>(GetBehaviorTreeComponent());
	assert(behaviorRangeComponent && "Invalid behaviorRangeComponent");

	behaviorRangeComponent->SetEnableFireRange(enableFireRange);
}

void NPCRangeBase::InitializeAbility()
{
	/* 원거리 공격 Ability 부여 */
	grantRangeAttackAbilityID = abilitySystemComponent->AddNewAbility(1001, 1);
	AbilityRange* grantedAttackAbility = abilitySystemComponent->GetAbility<AbilityRange>(grantRangeAttackAbilityID);
	assert(grantedAttackAbility && "Invalid grantedAttackAbility");
	grantedAttackAbility->SetAttackFrame(attackDuration, attackFrameTime);
}

void NPCRangeBase::ExecuteAttack()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponentPtr = GetAbilitySystemComponent();
	assert(abilitySystemComponentPtr && "Invalid abilitySystemComponent");

	abilitySystemComponentPtr->ActivateAbility(grantRangeAttackAbilityID);
}

void NPCRangeBase::OnNotifyAttackFrame(const AbilityObject& ability)
{
	std::shared_ptr<Pawn> chaseTarget = GetChaseTarget();
	if (!chaseTarget || chaseTarget->IsDeath())
	{
		return;
	}

	/* 현재 위치 */
	const Vector2Int& ownerLocation = GetWorldPosition();

	/* 타겟의 위치 */
	const Vector2Int& targetLocation = chaseTarget->GetWorldPosition();

	/* owner와 target의 위치 비교(동일하면 안됨) */
	if (ownerLocation == targetLocation)
	{
		return;
	}

	/* 조준 방향(타겟을 향하게) */
	Vector2Float toTargetDirection = static_cast<Vector2Float>(targetLocation - ownerLocation);
	toTargetDirection.Normalize();
	assert(toTargetDirection != Vector2Float::Zero && "toTargetDirection is zero..");

	/* 현재 위치에서 타겟을 향해 발사체 생성 */
	SpawnProjectile(ownerLocation, toTargetDirection);
}
