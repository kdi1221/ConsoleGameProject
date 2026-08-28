#include "ProjectileFrozenOrb.h"
#include "ProjectileIceBolt.h"
#include <Math/Vector2Float.h>
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

ProjectileFrozenOrb::ProjectileFrozenOrb(const Vector2Int& inPosition, 
										const float moveSpeed, 
										const eTeamID teamID, 
										float damageValue,
										float spawnIceboltDelay)
	:super(inPosition, L"•", Color::LightBlue, moveSpeed, teamID, damageValue)
{
	SetCollisionDestroyFlags(eProjectileCollisionFlags::BlockWall);

	timerSpawnIceBoltDelay.SetTargetTime(spawnIceboltDelay);
	timerSpawnIceBoltDelay.ReserveNextTick();
}

void ProjectileFrozenOrb::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	/* 오브 구체 시간 누적 */
	accumulationTime += deltaTime;

	/* 얼음살 발사 딜레이 타이머 */
	timerSpawnIceBoltDelay.Tick(deltaTime);
	if (timerSpawnIceBoltDelay.IsTimeOut())
	{
		SpawnIceBolts();

		timerSpawnIceBoltDelay.Reset();
	}
}

void ProjectileFrozenOrb::SetSpawnIceBoltNum(int IceBoltNum)
{
	spawnIceBoltNum = IceBoltNum;
}

void ProjectileFrozenOrb::SetSpawnIceBoltMoveSpeed(float moveSpeed)
{
	moveSpeedIceBolt = moveSpeed;
}

void ProjectileFrozenOrb::SetSpawnIceBoltDamage(float damageAmount)
{
	damageIceBolt = damageAmount;
}

void ProjectileFrozenOrb::SpawnIceBolts()
{
	std::shared_ptr<Level> ownerLevel = GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* IceBolt 의 스폰 위치 */
	const Vector2Int spawnPosition = GetWorldPosition();

	/* Instigator의 Team ID*/
	const eTeamID instigatorTeamID = GetInstigatorTeamID();

	/* 현재 구체의 이동방향 */
	const Vector2Float moveDirection = GetMoveDirection();

	/* 생성할 얼음살의 이동방향 회전 각도 */
	const float spawnIceBoltFireAngle = 360.f / static_cast<float>(spawnIceBoltNum);
	
	/* 얼음살 방향 회전 각도 오프셋 = 오브 구체 누적시간 * 회전 속도*/
	const float addAngleOffset = accumulationTime * rotateSpeed;

	for (int i = 0; i < spawnIceBoltNum; ++i)
	{
		/* 방향 회전 각도 오프셋 + 현재 생성할 얼음살의 이동방향 각도 */
		const float currentDirectionAngle = addAngleOffset + (i * spawnIceBoltFireAngle);

		/* 현재 생성될 얼음살의 최종 이동 방향 */
		const Vector2Float rotateDirection = Vector2Float::RotateVector(moveDirection, currentDirectionAngle);

		/* 얼음살 생성 */
		std::shared_ptr<ProjectileIceBolt> spawnedIceBolt = ownerLevel->SpawnActor<ProjectileIceBolt>(
			spawnPosition,
			moveSpeedIceBolt,
			instigatorTeamID,
			damageIceBolt);

		assert(spawnedIceBolt && "Spawn Fail IceBolt");

		/* 생성된 얼음살의 이동 방향 지정 */
		spawnedIceBolt->SetMoveDirection(rotateDirection);

		/* 생성된 얼음살의 LifeSpan 지정 */
		spawnedIceBolt->SetLifeSpan(1.f);
	}

}
