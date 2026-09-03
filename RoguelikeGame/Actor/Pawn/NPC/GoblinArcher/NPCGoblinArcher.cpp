#include "NPCGoblinArcher.h"
#include "Actor/Projectile/Projectile.h"
#include <Level/Level.h>
#include <cassert>

using namespace Craft;

NPCGoblinArcher::NPCGoblinArcher(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"g", Color::Green, 15.f, 10.f, 2.f, 0.8f, 10.f, roomIndex)
{

}

void NPCGoblinArcher::SpawnProjectile(const Vector2Int& spawnPosition, const Vector2Float& aimingDireciton)
{
	std::shared_ptr<Level> ownerLevel = GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	/* NPC의 Team ID */
	eTeamID instigatorTeamID = GetTeamID();

	//발사체 생성
	std::shared_ptr<Projectile> spawnedProjectile = ownerLevel->SpawnActor<Projectile>(
		spawnPosition,
		20.f,
		instigatorTeamID,
		3.f);

	assert(spawnedProjectile && "Spawn Fail Projectile..");

	/* 생성된 발사체의 이미지 지정 */
	spawnedProjectile->SetImage(L"*");

	/* 생성된 발사체의 이미지 색상 지정 */
	spawnedProjectile->SetImageColor(Color::Red);

	/* 생성된 발사체의 이동 방향 지정 */
	spawnedProjectile->SetMoveDirection(aimingDireciton);

	/* 생성된 발사체의 지속시간 지정 */
	spawnedProjectile->SetLifeSpan(2.f);
}