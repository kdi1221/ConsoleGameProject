#include "BehaviorMeleeComponent.h"
#include "Actor/Pawn/NPC/NPCBase.h"
#include <Navigation/NavigationTilemap.h>
#include <Engine/Engine.h>

using namespace Craft;

bool BehaviorMeleeComponent::IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const
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

	return abs(distance.x) <= 1 && abs(distance.y) <= 1;
}

void BehaviorMeleeComponent::GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, std::vector<Craft::Vector2Int>& availablePosition)
{
	std::shared_ptr<NPCBase> npcOwnerPtr = GetNPCOwner();
	if (!npcOwnerPtr)
	{
		return;
	}

	const RoomDefines::UNIQUE_INDEX_TYPE npcPlaceRoomIndex = npcOwnerPtr->GetSpawnedRoomIndex();

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

			//TODO : 대각 이동 체크?

			if (!navigationSystem.CanNextMove(npcOwnerPtr, checkPos))
			{
				continue;
			}

			//생성된 방 내의 타일이어야만 가능
			if (npcPlaceRoomIndex != navigationSystem.GetRoomIndexInTile(checkPos))
			{
				continue;
			}

			availablePosition.emplace_back(checkPos);
		}
	}
}
