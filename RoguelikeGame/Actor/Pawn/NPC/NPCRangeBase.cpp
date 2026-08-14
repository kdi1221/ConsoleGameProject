#include "NPCRangeBase.h"
#include "Component/AbilitySystemComponent.h"
#include "Ability/Shooter/AbilityProjectile.h"
#include "Engine/Engine.h"
#include "Navigation/NavigationTilemap.h"
#include <Math/Vector2Float.h>
#include <cassert>

using namespace Craft;
NPCRangeBase::NPCRangeBase(const Craft::Vector2Int& position, 
							const std::wstring& image, 
							Craft::Color color, 
							float initialHealth, 
							RoomDefines::UNIQUE_INDEX_TYPE roomIndex, 
							float moveDelay, 
							float chaseDelay)
	:super(position, image, color, initialHealth, roomIndex, moveDelay, chaseDelay)
{

}

void NPCRangeBase::InitializeAbility()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	/* Projectile 발사 Ability 추가 */
	grantRangeAttackID = abilitySystemComponent->AddNewAbility<AbilityProjectile>(projectileFireDelay,
																					projectileImage,
																					projectileColor,
																					projectileMoveDelayMin,
																					projectileMoveDelayMax,
																					GetTeamID(),
																					projectileDamageValue);
}

void NPCRangeBase::GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, std::vector<Craft::Vector2Int>& availablePosition)
{
	const NavigationTilemap& navigationSystem = Engine::Get().GetNavigationSystem<NavigationTilemap>();

	/* 타겟의 위치를 중심으로 사정거리의 70 ~ 80프로 사이의 타일들을 추출한다. */
	const Vector2Int& currentPos = GetWorldPosition();
	const int radius = static_cast<int>(round(attackRange));
	float minRange = attackRange * 0.7f;
	minRange *= minRange;
	float maxRange = attackRange * 0.9f;
	maxRange *= maxRange;
	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}

			const Vector2Int distance(x, y);
			const float distanceLength = static_cast<float>(distance.Length());

			if (distanceLength >= minRange && distanceLength <= maxRange)
			{
				const Vector2Int checkPos(targetPos + Vector2Int(x, y));
				if (!navigationSystem.CanNextMove(shared_from_this(), checkPos))
				{
					continue;
				}

				//생성된 방 내의 타일이어야만 가능
				if (GetSpawnedRoomIndex() != navigationSystem.GetRoomIndexInTile(checkPos))
				{
					continue;
				}

				//타겟이 자신을 바라보는 방향과 90도 내에 있어야 함(뒤쪽으로 돌아가는 부분 방지)
				Vector2Float targetToCheck = static_cast<Vector2Float>(checkPos - targetPos);
				targetToCheck.Normalize();
				Vector2Float targetToNPC = static_cast<Vector2Float>(currentPos - targetPos);
				targetToNPC.Normalize();
				const float dotProduct = targetToCheck.DotProduct(targetToNPC);
				if (dotProduct < 0.f)
				{
					continue;
				}

				availablePosition.emplace_back(checkPos);
			}
		}
	}
}

bool NPCRangeBase::IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const
{
	if (!targetPawn)
	{
		return false;
	}

	const Vector2Int& targetPos = targetPawn->GetWorldPosition();
	const Vector2Int& worldPos = GetWorldPosition();
	const Vector2Int distance = targetPos - worldPos;

	return (attackRange * attackRange) >= distance.Length();
}

void NPCRangeBase::AttackAbilitiesTriggerON()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");
	AbilityProjectile* projectileAbility = abilitySystemComponent->GetAbility<AbilityProjectile>(grantRangeAttackID);
	if (!projectileAbility)
	{
		return;
	}

	std::shared_ptr<Pawn> target = GetChaseTarget().lock();
	if (!target)
	{
		return;
	}

	AdjustAimDirection();

	projectileAbility->TriggerOn();
}

void NPCRangeBase::AttackAbilitiesTriggerOFF()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	AbilityProjectile* projectileAbility = abilitySystemComponent->GetAbility<AbilityProjectile>(grantRangeAttackID);
	if (!projectileAbility)
	{
		return;
	}

	projectileAbility->TriggerOff();
}

void NPCRangeBase::OnBehaviorAttack(float deltaTime)
{
	super::OnBehaviorAttack(deltaTime);

	/* 앞선 비헤이비어 과정을 거친뒤 더이상 Attack 상태가 아니면 반환 */
	if (NPCBase::eMonsterBehavior::Attack != GetBehaviorState())
	{
		return;
	}

	/* 갱신된 타겟의 위치를 확인해서 조준 방향등을 조절해준다. */
	AdjustAimDirection();
}

void NPCRangeBase::AdjustAimDirection()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");
	AbilityProjectile* projectileAbility = abilitySystemComponent->GetAbility<AbilityProjectile>(grantRangeAttackID);
	if (!projectileAbility)
	{
		return;
	}

	std::shared_ptr<Pawn> target = GetChaseTarget().lock();
	if (!target)
	{
		return;
	}

	const Vector2Int& currentPosition = GetWorldPosition();
	const Vector2Int& targetPosition = target->GetWorldPosition();

	Vector2Float toTargetDistance = static_cast<Vector2Float>(targetPosition - currentPosition);
	toTargetDistance.Normalize();

	/* 가장 적절한 Offset 방향을 구한다. */
	float bestOffsetDot = FLT_MIN;
	Vector2Int bestOffset = Vector2Int::Zero;
	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}

			Vector2Float toOffsetDirection(static_cast<float>(x), static_cast<float>(y));
			toOffsetDirection.Normalize();

			const float DotResult = toOffsetDirection.DotProduct(toTargetDistance);
			if(DotResult > bestOffsetDot)
			{ 
				bestOffsetDot = DotResult;
				bestOffset.x = x;
				bestOffset.y = y;
			}
		}
	}

	/* 타겟을 향해 발사할때 가장 적절한 Offset 선택 */
	projectileAbility->SetProjectileSpawnOffset(bestOffset);
	projectileAbility->SetAimingPostion(targetPosition);

	/* offset 위치에서 타겟까지의 방향을 확인해서 조준 위치 설정 */
	//const Vector2Int firePosition = currentPosition + bestOffset;

	//if (targetPosition == firePosition)
	//{
	//	/* 타겟의 위치와 발사할 위치가 동일한 경우 타겟의 위치로 조준위치를 설정한다. */
	//	projectileAbility->SetAimingPostion(targetPosition);
	//}
	//else
	//{
	//	Vector2Float offsetToTargetDistance = static_cast<Vector2Float>(targetPosition - firePosition);
	//	offsetToTargetDistance.Normalize();
	//	const Vector2Int aimingPosition = firePosition + static_cast<Vector2Int>(offsetToTargetDistance * attackRange);
	//	projectileAbility->SetAimingPostion(aimingPosition);
	//}
}
