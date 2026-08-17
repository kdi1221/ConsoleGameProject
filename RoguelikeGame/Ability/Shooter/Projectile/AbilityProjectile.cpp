#include "AbilityProjectile.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/Projectile/Projectile.h"
#include <Math/Vector2Float.h>
#include <Level/Level.h>
#include <Util/Util.h>
#include <cassert>

using namespace Craft;

AbilityProjectile::AbilityProjectile(int id,
									int level,
									float fireDelay,
									const std::wstring image,
									const Craft::Color color,
									float moveDelayMin,
									float moveDelayMax,
									eTeamID teamID,
									float damageValue)
	:super(id, level, fireDelay)
	,projectileImage(image)
	,projectileColor(color)
	,projectileMoveDelayMin(moveDelayMin)
	,projectileMoveDelayMax(moveDelayMax)
	,instigatorTeamID(teamID)
	,projectileDamage(damageValue)
{

}

void AbilityProjectile::PlayShootSound()
{

}

void AbilityProjectile::OnIntervalTrigger()
{
	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invlid ownerActor");

	std::shared_ptr<Level> ownerLevel = ownerPawn->GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	//Projectile Spawn Position Offset(Actor Pos + Offset)
	const Vector2Int& spawnOffset = ownerPawn->GetProjectileSpawnOffset();
	const Vector2Int spawnPosition = ownerPawn->GetWorldPosition() + spawnOffset;

	//발사체의 이동 딜레이(랜덤 지정)
	const float projectileMoveDelay = Util::RandomRange(projectileMoveDelayMin, projectileMoveDelayMax);

	//스킬 레벨에 따른 추가 발사체 스폰
	static const float spreadAngle = 15.f;
	std::vector<float> fireAngles = { 0.f };
	const int currentLevel = GetAbilityLevel();
	for (int i = 1; i < currentLevel; ++i)
	{
		const float currentSpreadAngle = spreadAngle * i;
		fireAngles.push_back(-currentSpreadAngle);
		fireAngles.push_back(+currentSpreadAngle);
	}

	const Vector2Int& aimingPosition = ownerPawn->GetAimingPosition();
	const Vector2Int toAimingDirection = aimingPosition - spawnPosition;
	const float aimDistance = static_cast<float>(toAimingDirection.LengthSqrt());

	for (float fireAngle : fireAngles)
	{
		Vector2Int projectileAimingPosition = aimingPosition;

		if (toAimingDirection == Vector2Int::Zero || aimDistance == 0.f)
		{
			/* 발사 위치와 조준 위치가 동일하면 그 위치 그대로 생성 */
			projectileAimingPosition = spawnPosition;
		}
		else
		{
			const Vector2Float rotateAimingDir = Vector2Float::RotateVector(static_cast<Vector2Float>(toAimingDirection), fireAngle);
			const Vector2Float rotateAimingOffset = rotateAimingDir * aimDistance;

			projectileAimingPosition = spawnPosition + Vector2Int(static_cast<int>(round(rotateAimingOffset.x)), static_cast<int>(round(rotateAimingOffset.y)));
		}

		ownerLevel->SpawnActor<Projectile>(spawnPosition,
			projectileImage,
			projectileColor,
			projectileAimingPosition,
			projectileMoveDelay,
			instigatorTeamID,
			projectileDamage);
	}

	PlayShootSound();
}
