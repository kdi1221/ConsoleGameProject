#include "AbilityShotSpiritBall.h"

#include "Actor/Projectile/Projectile.h"
#include "Actor/Pawn/Pawn.h"
#include <Level/Level.h>
#include <cassert>


#include <windows.h>

using namespace Craft;

AbilityShotSpiritBall::AbilityShotSpiritBall(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{

}

AbilityShotSpiritBall::~AbilityShotSpiritBall()
{

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

	//Projectile 스폰
	ownerLevel->SpawnActor<Projectile>(spawnPosition,
										spawnProjectileImage,
										spawnProjectileColor);

	EndAbility(false);
}

void AbilityShotSpiritBall::EndAbility(bool bCancelAbility)
{
	super::EndAbility(bCancelAbility);

	OutputDebugStringA("TODO : End Ability\n");
}

void AbilityShotSpiritBall::CancelAbility()
{
	super::CancelAbility();
}
