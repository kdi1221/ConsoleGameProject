#include "AbilityShotSpiritBall.h"
#include "Actor/Projectile/SpiritBall/ProjectileSpiritBall.h"
#include "Actor/Pawn/Pawn.h"
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

AbilityShotSpiritBall::AbilityShotSpiritBall(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	SetCooldownTime(0.5f);
	SetManaCost(5.f);
}

void AbilityShotSpiritBall::ActivateAbility()
{
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

	//스킬 레벨에 따른 추가 발사체 스폰
	static const float spreadAngle = 15.f;
	std::vector<float> fireAngles = { 0.f, -spreadAngle, spreadAngle };
	
	const int currentLevel = GetAbilityLevel();
	for (int i = 1; i < currentLevel; ++i)
	{
		const float currentSpreadAngle = spreadAngle * (i + 1);
		fireAngles.push_back(-currentSpreadAngle);
		fireAngles.push_back(+currentSpreadAngle);
	}

	for (float fireAngle : fireAngles)
	{
		/* 현재 각도에서의 발사 방향 */
		const Vector2Float rotateShootDirection = Vector2Float::RotateVector(static_cast<Vector2Float>(aimingDirection), fireAngle);

		//Projectile 스폰
		std::shared_ptr<ProjectileSpiritBall> spawnedProjectile = ownerLevel->SpawnActor<ProjectileSpiritBall>(
			spawnPosition,
			60.f,
			instigatorTeamID,
			10.f);

		assert(spawnedProjectile && "Spawn Fail Projectile");

		/* 생성된 Projectile의 이동 방향 지정 */
		spawnedProjectile->SetMoveDirection(rotateShootDirection);

		/* 생성된 Projectile의 LifeSpan 지정 */
		spawnedProjectile->SetLifeSpan(1.f);
	}


	EndAbility(false);
}