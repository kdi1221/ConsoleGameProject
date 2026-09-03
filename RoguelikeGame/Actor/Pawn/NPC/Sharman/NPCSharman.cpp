#include "NPCSharman.h"
#include "Actor/Projectile/Projectile.h"
#include <Util/Util.h>
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

NPCSharman::NPCSharman(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"S", Color::Yellow, 50.f, 10.f, 1.5f, 0.9f, 12.f, roomIndex)
{
	
}

void NPCSharman::SpawnProjectile(const Craft::Vector2Int& spawnPosition, const Craft::Vector2Float& aimingDireciton)
{
	std::shared_ptr<Level> ownerLevel = GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* NPC의 Team ID */
	eTeamID instigatorTeamID = GetTeamID();

	/* 랜덤하게 추가 발사체 스폰 */
	static const float spreadAngle = 15.f;
	std::vector<float> fireAngles = { 0.f };
	const int spawnNum = Util::RandomRange(1, 3);
	for (int i = 1; i < spawnNum; ++i)
	{
		const float currentSpreadAngle = spreadAngle * i;
		fireAngles.push_back(-currentSpreadAngle);
		fireAngles.push_back(+currentSpreadAngle);
	}

	for (float fireAngle : fireAngles)
	{
		const Vector2Float rotateAimingDir = Vector2Float::RotateVector(static_cast<Vector2Float>(aimingDireciton), fireAngle);

		//발사체 생성
		std::shared_ptr<Projectile> spawnedProjectile = ownerLevel->SpawnActor<Projectile>(
			spawnPosition,
			20.f,
			instigatorTeamID,
			8.f);

		assert(spawnedProjectile && "Spawn Fail Projectile..");

		/* 생성된 발사체의 이미지 지정 */
		spawnedProjectile->SetImage(L"○");

		/* 생성된 발사체의 이미지 색상 지정 */
		spawnedProjectile->SetImageColor(Color::DarkGray);

		/* 생성된 발사체의 이동 방향 지정 */
		spawnedProjectile->SetMoveDirection(rotateAimingDir);

		/* 생성된 발사체의 지속시간 지정 */
		spawnedProjectile->SetLifeSpan(2.f);
	}
}
