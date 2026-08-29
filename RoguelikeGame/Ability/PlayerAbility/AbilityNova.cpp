#include "AbilityNova.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/SkillObject/AbilityNovaActor.h"
#include <Engine/Engine.h>
#include <Level/Level.h>

using namespace Craft;

AbilityNova::AbilityNova(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{	
	SetCooldownTime(0.5f);
	SetManaCost(5.f);
}

void AbilityNova::ActivateAbility()
{
	super::ActivateAbility();

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerActor");

	std::shared_ptr<Level> ownerLevel = ownerPawn->GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* owner의 위치 */
	const Vector2Int spawnPosition = ownerPawn->GetWorldPosition();

	/* owner의 Team ID*/
	eTeamID instigatorTeamID = ownerPawn->GetTeamID();

	//노바 객체 생성
	std::shared_ptr<AbilityNovaActor> spawnedNovaActor = ownerLevel->SpawnActor<AbilityNovaActor>(
		spawnPosition,
		instigatorTeamID,
		60.f,
		2.f,
		10.f,
		10.f);

	assert(spawnedNovaActor && "Spawn Fail Nova Actor");

	/* 사운드 재생 */
	Engine::Get().PlayOneShot("Effect/eleccast.wav");

	EndAbility(false);
}
