#include "AbilityProjectile.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/Projectile/Projectile.h"
#include <Level/Level.h>
#include <Util/Util.h>
#include <cassert>

using namespace Craft;

AbilityProjectile::AbilityProjectile(float fireDelay,
									const std::wstring image,
									const Craft::Color color,
									float moveDelayMin,
									float moveDelayMax,
									eTeamID teamID,
									float damageValue)
	:super(fireDelay)
	,projectileImage(image)
	,projectileColor(color)
	,projectileMoveDelayMin(moveDelayMin)
	,projectileMoveDelayMax(moveDelayMax)
	,instigatorTeamID(teamID)
	,projectileDamage(damageValue)
{

}

void AbilityProjectile::SetProjectileSpawnOffset(const Craft::Vector2Int& spawnOffset)
{
	projectileSpawnOffset = spawnOffset;
}

void AbilityProjectile::SetAimingPostion(const Craft::Vector2Int& position)
{
	aimingPosition = position;
}

void AbilityProjectile::OnIntervalTrigger()
{
	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invlid ownerActor");

	std::shared_ptr<Level> ownerLevel = ownerPawn->GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	//Projectile Spawn Position Offset(Actor Pos + Offset)
	const Vector2Int spawnPosition = ownerPawn->GetWorldPosition() + projectileSpawnOffset;

	//발사체의 이동 딜레이(랜덤 지정)
	const float projectileMoveDelay = Util::RandomRange(projectileMoveDelayMin, projectileMoveDelayMax);
	ownerLevel->SpawnActor<Projectile>(spawnPosition,
		projectileImage,
		projectileColor,
		aimingPosition,
		projectileMoveDelay,
		instigatorTeamID,
		projectileDamage);
}
