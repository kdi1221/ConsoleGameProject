#include "AbilityEnergyBeam.h"
#include "Level/TilemapLevel.h"
#include "Tilemap/Room/Room.h"
#include "Tilemap/BSP/RoomSpace/RoomSpace.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/SkillObject/AbilityEnergyBeamObject.h"
#include <cassert>

using namespace Craft;

AbilityEnergyBeam::AbilityEnergyBeam(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{

}

void AbilityEnergyBeam::ActivateAbility()
{
	super::ActivateAbility();

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerPawn");

	std::shared_ptr<TilemapLevel> currentLevel = Cast<TilemapLevel>(ownerPawn->GetOwner());
	assert(currentLevel && "Invalid currentLevel");

	const Room* locatedRoom = currentLevel->GetPostionInRoom(ownerPawn->GetPosition());
	assert(locatedRoom && "Invalid locatedRoom");

	/* owner의 Team ID*/
	const eTeamID instigatorTeamID = ownerPawn->GetTeamID();

	/* 현재 위치한 방의 영역 정보 조회 */
	const RoomSpace& locatedRoomSpace = locatedRoom->GetRoomSpace();
	const int roomWidth = locatedRoomSpace.GetWidth();
	const int roomHeight = locatedRoomSpace.GetHeight();

	/* 좌상단 위치 */
	const Vector2Int roomLTPos = locatedRoomSpace.GetPostionLT();

	/* 우상단 위치 */
	const Vector2Int roomRTPos(roomLTPos.x + roomWidth - 1, roomLTPos.y);

	/* 좌하단 위치 */
	const Vector2Int roomLBPos(roomLTPos.x, roomLTPos.y + roomHeight - 1);

	/* 우하단 위치 */
	const Vector2Int roomRBPos(roomLTPos.x + roomWidth - 1, roomLTPos.y + roomHeight - 1);

	/* 좌상단 -> 좌하단 도착 위치 */
	const Vector2Int distanceLTtoLB = roomLBPos - Vector2Int(0, maxEnergyBeamHeight);

	/* 좌하단 -> 우하단 도착 위치 */
	const Vector2Int distanceLBtoRB = roomRBPos - Vector2Int(maxEnergyBeamWidth, 0);

	/*우하단 -> 우상단 도착 위치 */
	const Vector2Int distanceRBtoRT = roomRTPos + Vector2Int(0, maxEnergyBeamHeight);

	/* 우상단 -> 좌상단 도착 위치*/
	const Vector2Int distanceRTtoLT = roomLTPos + Vector2Int(maxEnergyBeamWidth, 0);
		
	const FEnergyBeamActorSpawnInfo beamActorSpawnInfos[] =	
	{
		FEnergyBeamActorSpawnInfo(roomLTPos, distanceLTtoLB, maxEnergyBeamWidth),
		FEnergyBeamActorSpawnInfo(roomLBPos, distanceLBtoRB, maxEnergyBeamHeight),
		FEnergyBeamActorSpawnInfo(roomRBPos, distanceRBtoRT, maxEnergyBeamWidth),
		FEnergyBeamActorSpawnInfo(roomRTPos, distanceRTtoLT, maxEnergyBeamHeight),
	};

	int energyBeamIndex = 1;
	for (const FEnergyBeamActorSpawnInfo& spawnInfo : beamActorSpawnInfos)
	{
		//에너지 빔 객체 생성
		std::shared_ptr<AbilityEnergyBeamObject> spawnedEnergyBeam = currentLevel->SpawnActor<AbilityEnergyBeamObject>(
			spawnInfo.spawnPosition,
			spawnInfo.endPosition,
			spawnInfo.beamVerticalLength);

		spawnedEnergyBeam->SetInstigatorTeamID(instigatorTeamID);
		spawnedEnergyBeam->SetIndex(energyBeamIndex);
		spawnedEnergyBeam->SetExpandDuration(0.3f);
		spawnedEnergyBeam->SetDamageProcessInterval(0.3f);
		spawnedEnergyBeam->SetDamageValue(20.f);
		spawnedEnergyBeam->SetOnDestroyBeamCallback(std::bind(&AbilityEnergyBeam::OnEnergyBeamObjectDestroyed, this, std::placeholders::_1));

		spawnedBeamActors.insert({ energyBeamIndex, spawnedEnergyBeam });

		++energyBeamIndex;
	}
}

void AbilityEnergyBeam::CancelAbility()
{
	for (auto& iterEnergyBeamObejct : spawnedBeamActors)
	{
		if (std::shared_ptr<AbilityEnergyBeamObject> spawendBeamObejct = iterEnergyBeamObejct.second)
		{
			spawendBeamObejct->SetOnDestroyBeamCallback(nullptr);
			spawendBeamObejct->Destroy();
		}	
	}

	super::CancelAbility();
}

void AbilityEnergyBeam::OnEnergyBeamObjectDestroyed(const AbilityEnergyBeamObject& destroyBeamObject)
{
	spawnedBeamActors.erase(destroyBeamObject.GetBeamIndex());

	/* 모든 에너지 빔 액터가 사라지면 Ability 종료 */
	if (spawnedBeamActors.empty())
	{
		EndAbility(false);
	}
}
