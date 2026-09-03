#include "BehaviorRangeComponent.h"
#include "Actor/Pawn/NPC/NPCBase.h"
#include <Navigation/NavigationTilemap.h>
#include <Engine/Engine.h>

using namespace Craft;

bool BehaviorRangeComponent::IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const
{
	if (!targetPawn)
	{
		return false;
	}

	std::shared_ptr<NPCBase> npcOwnerPtr = GetNPCOwner();
	if (!npcOwnerPtr)
	{
		return false;
	}

	const Vector2Int& targetPos = targetPawn->GetWorldPosition();
	const Vector2Int& worldPos = npcOwnerPtr->GetWorldPosition();
	const Vector2Int distance = targetPos - worldPos;

	return ((enableFireRange * enableFireRange) >= distance.Length());
}

void BehaviorRangeComponent::GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, std::vector<Craft::Vector2Int>& availablePosition)
{
	std::shared_ptr<NPCBase> npcOwnerPtr = GetNPCOwner();
	if (!npcOwnerPtr)
	{
		return;
	}

	const RoomDefines::UNIQUE_INDEX_TYPE npcPlaceRoomIndex = npcOwnerPtr->GetSpawnedRoomIndex();

	const NavigationTilemap& navigationSystem = Engine::Get().GetNavigationSystem<NavigationTilemap>();

	/* 타겟의 위치를 중심으로 사정거리의 70 ~ 90프로 사이의 타일들을 추출한다. */
	const Vector2Int& currentPos = npcOwnerPtr->GetWorldPosition();
	const int radius = static_cast<int>(round(enableFireRange));
	float minRange = enableFireRange * 0.7f;
	minRange *= minRange;
	float maxRange = enableFireRange * 0.9f;
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

				//대각 이동 까지 체크
				if (!navigationSystem.SimulatePreviousToNextMove(npcOwnerPtr, currentPos, checkPos))
				{
					continue;
				}

				/*if (!navigationSystem.CanNextMove(npcOwnerPtr, checkPos))
				{
					continue;
				}*/

				//생성된 방 내의 타일이어야만 가능
				if (npcPlaceRoomIndex != navigationSystem.GetRoomIndexInTile(checkPos))
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

void BehaviorRangeComponent::SetEnableFireRange(float range)
{
	enableFireRange = range;
}
