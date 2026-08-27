#include "ProjectileMoveComponent.h"
#include "Actor/Actor.h"
#include <cassert>

namespace Craft
{
	ProjectileMoveComponent::ProjectileMoveComponent(float moveSpeed)
		:moveSpeed(moveSpeed)
	{

	}

	void ProjectileMoveComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		/* 실제 이동할 방향이 있을때만 이동 연산 */
		if (currentMoveDirection != Vector2Float::Zero)
		{
			UpdateMovement(deltaTime);
		}
	}

	void ProjectileMoveComponent::SetMoveDirection(const Vector2Float& newDirection)
	{
		currentMoveDirection = newDirection;
	}

	void ProjectileMoveComponent::ResetAccumulatorMoveDelta()
	{
		currentMoveDirection = Vector2Float::Zero;
	}

	void ProjectileMoveComponent::UpdateMovement(const float deltaTime)
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "ownerActor is invalid");

		/* 직전까지의 타일의 좌표 */
		const Vector2Int prevTileCoord = ownerActor->GetWorldPosition();

		/* 직전까지의 Delta */
		const Vector2Float prevDelta = accumulatorMoveDelta;

		/* 이동값 계산(방향 * 속도 * 델타타임) */
		const float moveDistance = moveSpeed * deltaTime;

		/* 현재 프레임 이동 델타 */
		const Vector2Float moveDelta = currentMoveDirection * moveDistance;

		/* 새로 누적된 이동 델타 */
		const Vector2Float newAccumulatorMoveDelta = accumulatorMoveDelta + moveDelta;

		/* 새로 누적된 이동 델타의 X, Y 축의 절대값이 1을 넘기지 않으면  */
		if (std::abs(newAccumulatorMoveDelta.x) < 1.f && abs(newAccumulatorMoveDelta.y) < 1.f)
		{
			/* 누적된 이동 델타값만 갱신 */
			accumulatorMoveDelta = newAccumulatorMoveDelta;
		}
		else
		{
			/* 현재 타일위치에 가산할 타일 델타 값 */
			const Vector2Int addTileCoordDelta(static_cast<int>(newAccumulatorMoveDelta.x),
												static_cast<int>(newAccumulatorMoveDelta.y));

			const float remainXAccumulatorDelta = newAccumulatorMoveDelta.x - static_cast<float>(addTileCoordDelta.x);
			const float remainYAccumulatorDelta = newAccumulatorMoveDelta.y - static_cast<float>(addTileCoordDelta.y);

			/* 이동할 타일 위치 좌표 */
			const Vector2Int nextTileCoord = prevTileCoord + addTileCoordDelta;

			/* 새로운 타일 좌표로 이동 */
			ownerActor->SetPosition(nextTileCoord);

			/* 누적 이동 델타값을 소수점 부분들만 남긴다. */
			accumulatorMoveDelta.x = remainXAccumulatorDelta;
			accumulatorMoveDelta.y = remainYAccumulatorDelta;
		}
	}
}