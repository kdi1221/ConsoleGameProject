#include "MovementComponent.h"
#include "Actor/Actor.h"
#include "Level/Level.h"
#include <cassert>

namespace Craft
{
	MovementComponent::MovementComponent(float inMoveDelay)
	{
		//이동 딜레이 타이머 설정
		moveTimer.SetTargetTime(inMoveDelay);
	}

	void MovementComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		/* 이동 방향이 존재할때만 이동 딜레이 타이머 작동 */
		if (lastMoveDirection != Vector2Int::Zero)
		{
			moveTimer.Tick(deltaTime);

			if (moveTimer.IsTimeOut())
			{
				UpdateMovement();
				moveTimer.Reset();
			}
		}
	}

	void MovementComponent::SetLastMoveDirection(const Vector2Int& moveDirection)
	{
		lastMoveDirection = moveDirection;
		if (lastMoveDirection == Vector2Int::Zero)
		{
			/* 이동 방향이 존재하지 않으면 타이머 초기화 */
			moveTimer.Reset();
		}
	}

	void MovementComponent::UpdateMovement()
	{
		/* 이동방향 벡터의 크기가 있을때만 작동 */
		if (lastMoveDirection == Vector2Int::Zero)
		{
			return;
		}

		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "ownerActor is invalid");

		/* 현재 위치(Local 기준) */
		const Vector2Int currentPosition = ownerActor->GetPosition();

		/* 이동할 새로운 위치(Local 기준) */
		const Vector2Int newPosition = currentPosition + lastMoveDirection;

		/* 이동이 실제 가능한지 점검 필요 */
		std::shared_ptr<Level> level = ownerActor->GetOwner();
		assert(level && "level is invalid");
		if (level->CanNextMove(ownerActor, newPosition))
		{
			/* 이동할 새로운 위치 지정 */
			ownerActor->SetPosition(newPosition);
		}
		else if((lastMoveDirection.x != 0) && (lastMoveDirection.y != 0))
		{
			/* 대각 이동인 상태에서 이동이 막혔던 경우 막혀있지 않은 한쪽 축 방향으로 이동한다. */
			const Vector2Int newXPosition = currentPosition + Vector2Int(lastMoveDirection.x, 0);
			const Vector2Int newYPosition = currentPosition + Vector2Int(0, lastMoveDirection.y);
			const bool canMoveXPos = level->CanNextMove(ownerActor, newXPosition);
			const bool canMoveYPos = level->CanNextMove(ownerActor, newYPosition);

			if (canMoveXPos && !canMoveYPos)
			{
				ownerActor->SetPosition(newXPosition);
			}
			else if (!canMoveXPos && canMoveYPos)
			{
				ownerActor->SetPosition(newYPosition);
			}
			else if (canMoveXPos && canMoveYPos)
			{
				/* 대각이동은 막혔으나 x, y 개별축으로는 이동이 가능한 상태 */
				/* 이 경우 x축으로 이동 */
				ownerActor->SetPosition(newXPosition);
			}
		}

		/* 이동 방향 벡터 초기화 */
		lastMoveDirection = Vector2Int::Zero;
	}
}