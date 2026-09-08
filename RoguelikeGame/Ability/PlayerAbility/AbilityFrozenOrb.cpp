#include "AbilityFrozenOrb.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/Projectile/FrozenOrb/ProjectileFrozenOrb.h"
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

AbilityFrozenOrb::AbilityFrozenOrb(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	SetCooldownTime(2.f);
	SetManaCost(20.f);
}

void AbilityFrozenOrb::ActivateAbility()
{
	super::ActivateAbility();

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerActor");

	std::shared_ptr<Level> ownerLevel = ownerPawn->GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* owner의 위치 */
	const Vector2Int ownerLocation = ownerPawn->GetWorldPosition();

	/* Projectile의 스폰 위치 */
	const Vector2Int spawnPosition = ownerLocation;

	/* owner의 Team ID*/
	eTeamID instigatorTeamID = ownerPawn->GetTeamID();

	/* owner의 조준 방향 */
	const Vector2Float aimingDirection = ownerPawn->GetAimingDirection();
	assert(aimingDirection != Vector2Float::Zero && "aimingDirection is zero..");

	float orbDamage = 20.f;
	float spawnIceBoltDelay = 0.2f;
	float orbLifeSpan = 1.5f;
	float iceBoltDamage = 8.f;
	int iceBoltSpawnNum = 5;
	switch (GetAbilityLevel())
	{
	case 1:
		orbDamage = 10.f;
		spawnIceBoltDelay = 0.4f;
		orbLifeSpan = 1.f;
		iceBoltDamage = 4.f;
		iceBoltSpawnNum = 3;
		break;

	case 2:
		orbDamage = 15.f;
		spawnIceBoltDelay = 0.3f;
		orbLifeSpan = 1.2f;
		iceBoltDamage = 6.f;
		iceBoltSpawnNum = 4;
		break;

	default:
		orbDamage = 25.f;
		spawnIceBoltDelay = 0.2f;
		orbLifeSpan = 1.5f;
		iceBoltDamage = 8.f;
		iceBoltSpawnNum = 5;
		break;
	}

	//오브 구체 생성
	std::shared_ptr<ProjectileFrozenOrb> spawnedOrbProjectile = ownerLevel->SpawnActor<ProjectileFrozenOrb>(
		spawnPosition,		
		15.f,
		instigatorTeamID,
		orbDamage,
		spawnIceBoltDelay);

	assert(spawnedOrbProjectile && "Spawn Fail Orb Projectile");

	/* 생성된 오브 구체의 이동 방향 지정 */
	spawnedOrbProjectile->SetMoveDirection(aimingDirection);

	/* 생성된 오브 구체의 지속시간 지정 */
	spawnedOrbProjectile->SetLifeSpan(orbLifeSpan);

	/* 오브 구체의 얼음살 생성 갯수 지정 */
	spawnedOrbProjectile->SetSpawnIceBoltNum(iceBoltSpawnNum);

	/* 생성되는 얼음살의 이동속도 지정 */
	spawnedOrbProjectile->SetSpawnIceBoltMoveSpeed(30.f);

	/* 생성되는 얼음살의 데미지 지정 */
	spawnedOrbProjectile->SetSpawnIceBoltDamage(iceBoltDamage);

	/* 사운드 재생 */
	Engine::Get().PlayOneShot("Effect/coldcast.wav");
	
	EndAbility(false);
}
