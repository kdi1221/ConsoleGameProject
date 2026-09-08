#include "AbilityNova.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/SkillObject/AbilityNovaActor.h"
#include <Engine/Engine.h>
#include <Level/Level.h>

using namespace Craft;

AbilityNova::AbilityNova(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{	
	SetCooldownTime(0.4f);
	SetManaCost(15.f);
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

	float extendSpeed = 60.f;
	float beginRange = 2.f;
	float limitRange = 10.f;
	float damage = 15.f;

	/* 레벨에 따른 노바 객체 생성 속성 조절 */
	const int abilityLevel = GetAbilityLevel();
	switch (abilityLevel)
	{
	case 1:
		extendSpeed = 60.f;
		beginRange = 2.f;
		limitRange = 10.f;
		damage = 15.f;
		break;

	case 2:
		extendSpeed = 63.f;
		beginRange = 3.f;
		limitRange = 12.f;
		damage = 25.f;
		break;

	default:
		extendSpeed = 65.f;
		beginRange = 4.f;
		limitRange = 14.f;
		damage = 40.f;
		break;
	}

	//노바 객체 생성
	std::shared_ptr<AbilityNovaActor> spawnedNovaActor = ownerLevel->SpawnActor<AbilityNovaActor>(
		spawnPosition,
		instigatorTeamID,
		extendSpeed,
		beginRange,
		limitRange,
		damage);

	assert(spawnedNovaActor && "Spawn Fail Nova Actor");

	/* 사운드 재생 */
	Engine::Get().PlayOneShot("Effect/eleccast.wav");

	EndAbility(false);
}
