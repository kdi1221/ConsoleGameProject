#include "AbilityFrozenOrb.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/Projectile/FrozenOrb/ProjectileFrozenOrb.h"
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

AbilityFrozenOrb::AbilityFrozenOrb(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	SetCooldownTime(1.f);
	SetManaCost(15.f);
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

	//오브 구체 생성
	std::shared_ptr<ProjectileFrozenOrb> spawnedOrbProjectile = ownerLevel->SpawnActor<ProjectileFrozenOrb>(
		spawnPosition,		
		20.f,
		instigatorTeamID,
		10.f,
		0.1f);

	assert(spawnedOrbProjectile && "Spawn Fail Orb Projectile");

	/* 생성된 오브 구체의 이동 방향 지정 */
	spawnedOrbProjectile->SetMoveDirection(aimingDirection);

	/* 생성된 오브 구체의 지속시간 지정 */
	spawnedOrbProjectile->SetLifeSpan(1.f);

	/* 오브 구체의 얼음살 생성 갯수 지정 */
	spawnedOrbProjectile->SetSpawnIceBoltNum(4);

	/* 생성되는 얼음살의 이동속도 지정 */
	spawnedOrbProjectile->SetSpawnIceBoltMoveSpeed(30.f);

	/* 생성되는 얼음살의 데미지 지정 */
	spawnedOrbProjectile->SetSpawnIceBoltDamage(5.f);
	
	EndAbility(false);
}
