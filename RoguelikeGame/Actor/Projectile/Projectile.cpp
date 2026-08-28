#include "Projectile.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/ProjectileMoveComponent.h"
#include "Types/Enums.h"
#include "Actor/Pawn/Pawn.h"
#include "Level/TilemapLevel.h"
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <cassert>

using namespace Craft;

Projectile::Projectile(const Vector2Int& inPosition,
						const std::wstring& inImage,
						Craft::Color inColor,
						const float moveSpeed,
						const eTeamID teamID,
						float damageValue)
	:super(inPosition)
	,prevPosition(inPosition)
	,instigatorTeamID(teamID)
	,damageValue(damageValue)
{
	spriteComponent = AddComponent<SpriteRendererComponent>(inImage, inColor, static_cast<int>(eRenderSortingOrder::Projectile));
	projectileMoveComponent = AddComponent<ProjectileMoveComponent>(moveSpeed);
}

void Projectile::PreTick(float deltaTime)
{
	super::PreTick(deltaTime);

	/* 이동 전 위치 저장 */
	prevPosition = GetWorldPosition();
}

void Projectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	/* 수명 타이머가 지정된 경우 타이머 진행 */
	if (isLifeSpan)
	{
		timerLifeSpan.Tick(deltaTime);
		if (timerLifeSpan.IsTimeOut())
		{
			/* 수명이 다하면 Destroy */
			Destroy();
		}
	}
}

void Projectile::PostTick(float deltaTime)
{
	super::PostTick(deltaTime);

	/* 활성화 상태인 경우 Collision 체크*/
	if (IsActive())
	{
		PostMoveCheckBlockCollision();
	}
}

void Projectile::SetMoveDirection(const Vector2Float& moveDirection)
{
	if (projectileMoveComponent)
	{
		projectileMoveComponent->SetMoveDirection(moveDirection);
	}
}

void Projectile::SetLifeSpan(float lifeTime)
{
	timerLifeSpan.SetTargetTime(lifeTime);
	isLifeSpan = true;
}

void Projectile::SetCollisionDestroyFlags(eProjectileCollisionFlags newFlags)
{
	collisionFlags = newFlags;
}

const Vector2Float& Projectile::GetMoveDirection() const
{
	if (projectileMoveComponent)
	{
		return projectileMoveComponent->GetMoveDirection();
	}

	return Vector2Float::Zero;
}

void Projectile::PostMoveCheckBlockCollision()
{
	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	/* 이동 후 위치 */
	const Vector2Int postMovePosition = GetWorldPosition();

	/* 브레젠험으로 이동 전 위치랑 현재 위치사이의 경로를 구한다. */
	std::vector<Vector2Int> movePaths;
	StaticFunctionLibrary::GetBresenhamPath(prevPosition, postMovePosition, movePaths);

	/* 구한 경로를 따라가면서 충돌 여부를 확인한다. */
	for (const Vector2Int& pathTileCoord : movePaths)
	{
		std::shared_ptr<ActorOnTile> blockingActor = nullptr;
		const CheckBlockingResult checkBlockingResult = tilemapLevel->CheckBlocking(shared_from_this(), pathTileCoord, blockingActor);
		
		bool isBlockDestroy = false;
		switch (checkBlockingResult)
		{
			//벽과 충돌
		case CheckBlockingResult::BlockWall:
			{
				isBlockDestroy = eProjectileCollisionFlags::None != (collisionFlags & eProjectileCollisionFlags::BlockWall);
			}
			break;

			//Actor와 충돌
		case CheckBlockingResult::BlockActor:
			{
				isBlockDestroy = eProjectileCollisionFlags::None != (collisionFlags & eProjectileCollisionFlags::BlockActor);

				std::shared_ptr<Pawn> blockingPawn = Cast<Pawn>(blockingActor);
				if (blockingPawn)
				{
					blockingPawn->TakeDamage(damageValue);
				}
			}
			break;

		default:
			{
				isBlockDestroy = false;
			}
			break;
		}

		if (isBlockDestroy)
		{
			/* 충돌된 상횡에서는 Destroy */
			Destroy();

			break;
		}
	}
}
