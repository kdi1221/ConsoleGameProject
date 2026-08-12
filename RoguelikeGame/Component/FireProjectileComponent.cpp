#include "FireProjectileComponent.h"
#include "Actor/Projectile/Projectile.h"
#include "Level/Level.h"
#include <Util/Util.h>
#include <cassert>
#include <windows.h>

using namespace Craft;

FireProjectileComponent::FireProjectileComponent(float fireInterval,
												const std::wstring& inProjectileImage,
												Color inProjectileColor,
												float inProjectileMoveDelayMin,
												float inProjectileMoveDelayMax,
												eTeamID teamID,
												int damage)
	:projectileImage(inProjectileImage)
	,projectileColor(inProjectileColor)
	,projectileMoveDelayMin(inProjectileMoveDelayMin)
	,projectileMoveDelayMax(inProjectileMoveDelayMax)
	,instigatorTeamID(teamID)
	,projectileDamage(damage)
{
	timerFireInterval.SetTargetTime(fireInterval);
}

void FireProjectileComponent::SetEnableFire(bool enable)
{
	if (enable == enableFire)
	{
		return;
	}
	
	enableFire = enable;

	/* 타이머 리셋 */
	timerFireInterval.Reset();
}

void FireProjectileComponent::SetProjectileSpawnOffset(const Craft::Vector2Int& offset)
{
	projectileSpawnOffset = offset;
}

void FireProjectileComponent::SetProjectileAimingPosition(const Craft::Vector2Int& position)
{
	projectileAimingPosition = position;
}

void FireProjectileComponent::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (enableFire)
	{
		timerFireInterval.Tick(deltaTime);

		if (timerFireInterval.IsTimeOut())
		{
			SpawnProjectile();
			timerFireInterval.Reset();
		}
	}
}

void FireProjectileComponent::SpawnProjectile()
{
	std::shared_ptr<Actor> ownerActor = GetOwner();
	assert(ownerActor && "Invlid ownerActor");

	std::shared_ptr<Level> ownerLevel = ownerActor->GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	//Projectile Spawn Position Offset(Actor Pos + Offset)
	const Vector2Int spawnPosition = ownerActor->GetWorldPosition() + projectileSpawnOffset;

	//발사체의 이동 딜레이(랜덤 지정)
	const float projectileMoveDelay = Util::RandomRange(projectileMoveDelayMin, projectileMoveDelayMax);
	ownerLevel->SpawnActor<Projectile>(spawnPosition, 
										projectileImage, 
										projectileColor, 
										projectileAimingPosition, 
										projectileMoveDelay,
										instigatorTeamID,
										projectileDamage);
}


