#include "AbilityVitalityCircle.h"
#include "Actor/Pawn/Pawn.h"
#include "Level/TilemapLevel.h"
#include "Actor/SkillObject/AbilityVitalityCircleObject.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

AbilityVitalityCircle::AbilityVitalityCircle(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	SetCooldownTime(4.f);
	SetManaCost(25.f);
}

bool AbilityVitalityCircle::CanActivateAbility() const
{
	if (!super::CanActivateAbility())
	{
		return false;
	}

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	if (!ownerPawn)
	{
		return false;
	}

	std::shared_ptr<TilemapLevel> currentLevel = Cast<TilemapLevel>(ownerPawn->GetOwner());
	if (!currentLevel)
	{
		return false;
	}

	/* Owner가 특정 방안에 있어야하고, 지정한 위치 또한 같은 방이어야 한다. */
	const Vector2Int& ownerPosition = ownerPawn->GetWorldPosition();
	RoomDefines::UNIQUE_INDEX_TYPE ownerRoomIndex = currentLevel->GetRoomIndexInTile(ownerPosition);
	if (RoomDefines::ROOM_INDEX_INVALID == ownerRoomIndex)
	{
		return false;
	}

	const Vector2Int& aimingLocation = ownerPawn->GetAimingLocation();
	RoomDefines::UNIQUE_INDEX_TYPE aimingLocationRoomIndex = currentLevel->GetRoomIndexInTile(aimingLocation);
	if (aimingLocationRoomIndex != ownerRoomIndex)
	{
		return false;
	}

	/* 지정한 위치가 벽타일이 아니어야 한다.*/
	if (eTileCategory::Ground != currentLevel->GetTileCategory(aimingLocation))
	{
		return false;
	}

	return true;
}


void AbilityVitalityCircle::ActivateAbility()
{
	super::ActivateAbility();

	/* 기존 Circle 제거 */
	SpawnedVitalityCircleDestroy();

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerActor");

	std::shared_ptr<TilemapLevel> currentLevel = Cast<TilemapLevel>(ownerPawn->GetOwner());
	assert(currentLevel && "Invalid currentLevel");

	/* 스폰 위치 */
	const Vector2Int& spawnLocation = ownerPawn->GetAimingLocation();

	/* owner의 Team ID*/
	eTeamID instigatorTeamID = ownerPawn->GetTeamID();

	float healthRegen = 7.f;
	float manaRegen = 6.f;
	float circleLifeSpan = 10.f;
	float intervalRegen = 0.4f;
	switch (GetAbilityLevel())
	{
	case 1:
		healthRegen = 3.f;
		manaRegen = 2.f;
		circleLifeSpan = 6.f;
		intervalRegen = 0.5f;
		break;

	case 2:
		healthRegen = 5.f;
		manaRegen = 4.f;
		circleLifeSpan = 8.f;
		intervalRegen = 0.5f;
		break;

	default:
		healthRegen = 7.f;
		manaRegen = 6.f;
		circleLifeSpan = 10.f;
		intervalRegen = 0.4f;
		break;
	}

	//지정한 위치에 회복진 생성
	std::shared_ptr<AbilityVitalityCircleObject> spawnedVitalityCircle = currentLevel->SpawnActor<AbilityVitalityCircleObject>(spawnLocation);
	spawnedVitalityCircle->SetInstigatorTeamID(instigatorTeamID);
	spawnedVitalityCircle->SetlimitRange(12.f);
	spawnedVitalityCircle->SetHealthRegeneration(healthRegen);
	spawnedVitalityCircle->SetManaRegeneration(manaRegen);
	spawnedVitalityCircle->SetLifeSpan(circleLifeSpan);
	spawnedVitalityCircle->SetIntervalRegeneration(intervalRegen);
	spawnedVitalityCircle->SetOnDestroyVitalityCircleCallback(std::bind(&AbilityVitalityCircle::OnSpawnedVitalityCircleActorDestroy, this, std::placeholders::_1));

	spawnedCircleObject = spawnedVitalityCircle;

	/* 사운드 재생 */
	Engine::Get().PlayOneShot("Effect/prayer.wav");

	EndAbility(false);
}

void AbilityVitalityCircle::CancelAbility()
{
	/* Ability취소할때 기존에 생성했던 마법진 제거 */
	SpawnedVitalityCircleDestroy();

	super::CancelAbility();
}

void AbilityVitalityCircle::OnSpawnedVitalityCircleActorDestroy(const AbilityVitalityCircleObject& destroyedCircle)
{
	spawnedCircleObject.reset();
}

void AbilityVitalityCircle::SpawnedVitalityCircleDestroy()
{
	std::shared_ptr<AbilityVitalityCircleObject> spawnedVitalityCircle = spawnedCircleObject.lock();
	if (!spawnedVitalityCircle)
	{
		return;
	}

	/* 기존에 생성했던 마법진에 연결된 콜백을 초기화한다.*/
	spawnedVitalityCircle->SetOnDestroyVitalityCircleCallback(nullptr);
	spawnedVitalityCircle->Destroy();

	/* Destroy 후 초기화 */
	spawnedCircleObject.reset();
}


