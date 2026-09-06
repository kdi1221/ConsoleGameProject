#include "AbilityShockWave.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/SkillObject/AbilityShockWaveActor.h"
#include <cassert>
#include <Level/Level.h>

using namespace Craft;

AbilityShockWave::AbilityShockWave(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	nextSpawnWaveDelay.SetTargetTime(2.f);
}

void AbilityShockWave::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (IsActivate())
	{
		nextSpawnWaveDelay.Tick(deltaTime);
		if (nextSpawnWaveDelay.IsTimeOut())
		{
			if (maxSpawnShockWave == currentSpawnShockWave)
			{
				EndAbility(false);
			}
			else
			{
				SpawnShockWaveActor();
				nextSpawnWaveDelay.Reset();
			}
		}
	}
}

void AbilityShockWave::ActivateAbility()
{
	super::ActivateAbility();

	currentSpawnShockWave = 0;
	nextSpawnWaveDelay.Reset();

	/* 최초 웨이브 생성 */
	SpawnShockWaveActor();
}

void AbilityShockWave::EndAbility(bool bCancelAbility)
{
	super::EndAbility(bCancelAbility);

	currentSpawnShockWave = 0;
	nextSpawnWaveDelay.Reset();
}

void AbilityShockWave::SpawnShockWaveActor()
{
	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerActor");

	std::shared_ptr<Level> ownerLevel = ownerPawn->GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* owner의 위치 */
	const Vector2Int spawnPosition = ownerPawn->GetWorldPosition();

	/* owner의 Team ID*/
	eTeamID instigatorTeamID = ownerPawn->GetTeamID();

	//노바 객체 생성
	std::shared_ptr<AbilityShockWaveActor> spawnedShockWave = ownerLevel->SpawnActor<AbilityShockWaveActor>(
		spawnPosition,
		instigatorTeamID,
		100.f,
		10.f,
		95.f,
		1.f,
		100.f);

	assert(spawnedShockWave && "Spawn Fail ShockWave Actor");

	++currentSpawnShockWave;
}
