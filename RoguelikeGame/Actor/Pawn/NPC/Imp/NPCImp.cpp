#include "NPCImp.h"
#include "Actor/Projectile/Projectile.h"
#include <Util/Util.h>
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

NPCImp::NPCImp(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"I", Color::Red, 35.f, 10.f, 1.5f, 0.75f, 15.f, roomIndex)
{

}

void NPCImp::SpawnProjectile(const Craft::Vector2Int& spawnPosition, const Craft::Vector2Float& aimingDireciton)
{
	std::shared_ptr<Level> ownerLevel = GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* NPC의 Team ID */
	eTeamID instigatorTeamID = GetTeamID();

	/* 랜덤하게 추가 발사체 스폰 */
	static const float spreadAngle = 10.f;
	std::vector<float> fireAngles = { 0.f };
	const int spawnNum = Util::RandomRange(1, 2);
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
			25.f,
			instigatorTeamID,
			5.f);

		assert(spawnedProjectile && "Spawn Fail Projectile..");

		/* 생성된 발사체의 이미지 지정 */
		spawnedProjectile->SetImage(L"•");

		/* 생성된 발사체의 이미지 색상 지정 */
		spawnedProjectile->SetImageColor(Color::Red);

		/* 생성된 발사체의 이동 방향 지정 */
		spawnedProjectile->SetMoveDirection(rotateAimingDir);

		/* 생성된 발사체의 지속시간 지정 */
		spawnedProjectile->SetLifeSpan(2.f);
	}
}
