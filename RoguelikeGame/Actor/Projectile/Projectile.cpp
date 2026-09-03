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
						const float moveSpeed,
						const eTeamID teamID,
						float damageValue)
	:super(inPosition)
	,prevPosition(inPosition)
	,instigatorTeamID(teamID)
	,damageValue(damageValue)
{
	projectileMoveComponent = AddComponent<ProjectileMoveComponent>(moveSpeed);

	spriteComponent = AddComponent<SpriteRendererComponent>(L"", Color::White, static_cast<int>(eRenderSortingOrder::Projectile));
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

void Projectile::SetImage(const std::wstring& image)
{
	if (spriteComponent)
	{
		spriteComponent->SetImage(image);
	}
}

void Projectile::SetImageColor(const Color color)
{
	if (spriteComponent)
	{
		spriteComponent->SetColor(color);
	}
}

void Projectile::SetSortingOrder(eRenderSortingOrder order)
{
	if (spriteComponent)
	{
		spriteComponent->SetSortingOrder(static_cast<int>(order));
	}
}

bool Projectile::OnBlockWall()
{
	return true;
}

bool Projectile::OnBlockActor(std::shared_ptr<ActorOnTile> blockingActor)
{
	std::shared_ptr<Pawn> blockingPawn = Cast<Pawn>(blockingActor);
	if (blockingPawn && blockingPawn->GetTeamID() != GetInstigatorTeamID())
	{
		/* 충돌한 다른 Actor가 Pawn이고 소속 팀이 다르면 데미지 적용 */
		blockingPawn->TakeDamage(damageValue);
	}

	return true;
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
				isBlockDestroy = OnBlockWall();
			}
			break;

			//Actor와 충돌
		case CheckBlockingResult::BlockActor:
			{
				isBlockDestroy = OnBlockActor(blockingActor);
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
