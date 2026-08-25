#include "MovementComponent.h"
#include "Actor/Actor.h"
#include "Navigation/NavigationBase.h"
#include "Engine/Engine.h"
#include <cassert>

namespace Craft
{
	const std::unordered_map<eDirection, Vector2Float> MOVE_DIRECTION =
	{
		{eDirection::Left, Vector2Float(-1.f, 0.f)},
		{eDirection::Top, Vector2Float(0.f, -1.f)},
		{eDirection::Right, Vector2Float(1.f, 0.f)},
		{eDirection::Bottom, Vector2Float(0.f, 1.f)},

		{eDirection::LeftTop, Vector2Float(-DIAGONAL_VALUE, -DIAGONAL_VALUE)},
		{eDirection::RightTop, Vector2Float(DIAGONAL_VALUE, -DIAGONAL_VALUE)},
		{eDirection::LeftBottom, Vector2Float(-DIAGONAL_VALUE, DIAGONAL_VALUE)},
		{eDirection::RightBottom, Vector2Float(DIAGONAL_VALUE, DIAGONAL_VALUE)},
	};

	MovementComponent::MovementComponent(float inMoveSpeed)
		:moveSpeed(inMoveSpeed)
	{

	}

	void MovementComponent::BeginPlay()
	{
		super::BeginPlay();

		ResetTempPostion();
	}

	void MovementComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		/* 실제 이동할 방향이 있을때만 이동 연산 */
		if (currentMoveDirection != eDirection::None)
		{
			UpdateMovement(deltaTime);
		}
	}

	void MovementComponent::SetLastMoveInputDireciton(const eDirection newDirection)
	{
		if (currentMoveDirection != newDirection)
		{
			//직전 이동방향과 현재 이동방향이 다르면 이동 위치 초기화
			ResetTempPostion();
		}

		/* 이동 방향 변경 */
		currentMoveDirection = newDirection;
	}

	void MovementComponent::ResetTempPostion()
	{
		if (std::shared_ptr<Actor> ownerActor = GetOwner())
		{
			/* Owner가 유효하면 Owner의 정수 인덱스 기반 위치를 실수형 위치로 초기화한다. */
			moveTempPosition = static_cast<Vector2Float>(ownerActor->GetWorldPosition());
		}
		else
		{
			/* Owner가 유효하지않으면 기본값으로 초기화한다. */
			moveTempPosition = Vector2Float::Zero;
		}
	}

	void MovementComponent::UpdateMovement(const float deltaTime)
	{
		//std::shared_ptr<Actor> ownerActor = GetOwner();
		//assert(ownerActor && "ownerActor is invalid");

		//const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();

		///* 직전까지의 타일의 좌표 */
		//const Vector2Int prevTilecoord = ownerActor->GetWorldPosition();

		///* 직전까지의 위치(실수형) */
		//const Vector2Float prevPosition = moveTempPosition;

		//auto movePosition = [&navigationSystem, this](std::shared_ptr<Actor> ownerActor, const Vector2Float& moveDirection, const float moveDistance)
		//	{
		//		/* 현재 위치한 타일의 좌표 */
		//		const Vector2Int currentTilecoord = ownerActor->GetWorldPosition();

		//		const Vector2Float moveDelta = moveDirection * moveDistance;

		//		/* 이동할 새로운 위치 */
		//		const Vector2Float newPosition = moveTempPosition + moveDelta;

		//		/* 점유할 타일 위치 */
		//		const Vector2Int newTileCoord(static_cast<int>(newPosition.x), static_cast<int>(newPosition.y));

		//		/* 이동할 타일위치가 현재위치와 다르면 */
		//		if (currentTilecoord == newTileCoord)
		//		{
		//			/* 이동 위치에 대해서만 갱신 */
		//			moveTempPosition = newPosition;

		//			return CheckMoveResultType::Success;
		//		}
		//		else
		//		{
		//			/* 이동 가능한 실제 타일 위치 */
		//			Vector2Int enableMoveTilcoord = newTileCoord;

		//			/* 직선경로를 따라 충돌되기 직전의 이동가능한 위치를 가져온다. */
		//			const CheckMoveResultType checkResult = navigationSystem.CheckEnableMoveToTargetPosition(ownerActor, newTileCoord, enableMoveTilcoord);

		//			/* 마지막으로 이동 가능한것으로 확인된 위치로 이동한다. */
		//			ownerActor->SetPosition(enableMoveTilcoord);

		//			if (enableMoveTilcoord == newTileCoord)
		//			{
		//				/* 다음 예상 위치와 실제 이동가능한 위치가 동일할때에는 이동 위치 갱신 */
		//				moveTempPosition = newPosition;
		//			}
		//			else
		//			{
		//				/* 다음 예상 위치와 실제 이동가능한 위치가 다를 경우 충돌된 상태이므로 이동 위치를 그 타일의 위치로 설정 */
		//				moveTempPosition = static_cast<Vector2Float>(enableMoveTilcoord);
		//			}

		//			return checkResult;
		//		}
		//	};

		///* 이동값 계산(방향 * 속도 * 델타타임) */
		//const Vector2Float& moveDirection = MOVE_DIRECTION.at(currentMoveDirection);
		//const float moveDistance = moveSpeed * deltaTime;

		///* 처음 이동 시도 */
		//const CheckMoveResultType moveResult = movePosition(ownerActor, moveDirection, moveDistance);

		//char szTmp[256] = { 0 };
		//sprintf_s(szTmp, "Prev moveTempPosition[%f %f], current moveTempPosition[%f %f]\n", prevPosition.x, prevPosition.y, moveTempPosition.x, moveTempPosition.y);
		//OutputDebugStringA(szTmp);

		///* 벽에 부딪치고 이동이 대각 이동인 경우 축을 분리해서 이동한다.(슬라이딩효과) */
		//if (CheckMoveResultType::BlockWall == moveResult && IsDiagonal(currentMoveDirection))
		//{
		//	/* 이동 후 현재 타일 위치 */
		//	const Vector2Int postMoveTilecoord = ownerActor->GetWorldPosition();

		//	//처리된 이동델타 계산(단 제자리에서 막혔을경우에는 이동한 거리는 0으로 처리한다.)
		//	const float processMoveDelta = (postMoveTilecoord == prevTilecoord) ? 0.f :(moveTempPosition - prevPosition).LengthSqrt();

		//	//실제 이동한 만큼은 빼고 남은 이동할 거리를 구한다.
		//	float remainMoveDistance = moveDistance - processMoveDelta;

		//	/* 남은 이동거리가 있으면 축 분리해서 막혀있지 않은쪽으로 움직인다. */
		//	if (remainMoveDistance > 0.f)
		//	{	
		//		const Vector2Int& addXDir = (eDirection::None != (currentMoveDirection & eDirection::Left)) ? Vector2Int::Left : Vector2Int::Right;
		//		const Vector2Int& addYDir = (eDirection::None != (currentMoveDirection & eDirection::Top)) ? Vector2Int::Up : Vector2Int::Down;
		//		const Vector2Int newXAxisTileCoord = postMoveTilecoord + addXDir;
		//		const Vector2Int newYAxisTileCoord = postMoveTilecoord + addYDir;

		//		const bool canMoveXAxis = navigationSystem.CanNextMove(ownerActor, newXAxisTileCoord);
		//		const bool canMoveYAXis = navigationSystem.CanNextMove(ownerActor, newYAxisTileCoord);

		//		if (canMoveXAxis && !canMoveYAXis)
		//		{
		//			//const Vector2Float& xAxisDirection = (eDirection::None != (currentMoveDirection & eDirection::Left)) ? Vector2Float::Left : Vector2Float::Right;
		//			//movePosition(ownerActor, xAxisDirection, remainMoveDistance);
		//		}
		//		else if (!canMoveXAxis && canMoveYAXis)
		//		{
		//			//const Vector2Float& yAxisDirection = (eDirection::None != (currentMoveDirection & eDirection::Top)) ? Vector2Float::Up : Vector2Float::Down;
		//			//movePosition(ownerActor, yAxisDirection, remainMoveDistance);
		//		}
		//		else if (canMoveXAxis && canMoveYAXis)
		//		{
		//			/* 대각이동은 막혔으나 x, y 개별축으로는 이동이 가능한 상태 */
		//			/* 이 경우 x축으로 이동 */
		//			//const Vector2Float& xAxisDirection = (eDirection::None != (currentMoveDirection & eDirection::Left)) ? Vector2Float::Left : Vector2Float::Right;
		//			//movePosition(ownerActor, xAxisDirection, remainMoveDistance);
		//		}
		//	}
		//}

std::shared_ptr<Actor> ownerActor = GetOwner();
assert(ownerActor && "ownerActor is invalid");

/* 현재 타일 위치 */
const Vector2Int& currentTilecoord = ownerActor->GetWorldPosition();

/* 직전까지의 위치(실수형) */
const Vector2Float prevPosition = moveTempPosition;

/* 이동값 계산(방향 * 속도 * 델타타임) */
const Vector2Float& moveDirection = MOVE_DIRECTION.at(currentMoveDirection);
const float moveDistance = moveSpeed * deltaTime;
const Vector2Float moveDelta = moveDirection * moveDistance;

/* 이동할 새로운 위치 */
const Vector2Float newPosition = prevPosition + moveDelta;

/* 점유할 타일 위치 */
const Vector2Int newTileCoord(static_cast<int>(newPosition.x), static_cast<int>(newPosition.y));

/* 이동할 타일위치가 현재위치와 다르면 */
if (currentTilecoord == newTileCoord)
{
	/* 이동 위치에 대해서만 갱신 */
	moveTempPosition = newPosition;
}
else
{
	const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();

	/* X축, Y축 이동 방향 계산 */
	const Vector2Int AxisDelta = (newTileCoord - currentTilecoord);
	const Vector2Int AxisAdd(AxisDelta.x != 0 ? AxisDelta.x / abs(AxisDelta.x) : 0,
		AxisDelta.y != 0 ? AxisDelta.y / abs(AxisDelta.y) : 0);

	Vector2Int enableMoveTilcoord = newTileCoord;

	/* 직선경로를 따라 충돌되기 직전의 이동가능한 위치를 가져온다. */
	const CheckMoveResultType checkResult = navigationSystem.CheckEnableMoveToTargetPosition(ownerActor, newTileCoord, enableMoveTilcoord);

	/* 마지막으로 이동 가능한것으로 확인된 위치로 이동한다. */
	ownerActor->SetPosition(enableMoveTilcoord);

	if (enableMoveTilcoord == newTileCoord)
	{
		/* 다음 예상 위치와 실제 이동가능한 위치가 동일할때에는 이동 위치 갱신 */
		moveTempPosition = newPosition;
	}
	else
	{
		/* 다음 예상 위치와 실제 이동가능한 위치가 다를 경우 충돌된 상태이므로 이동 위치 초기화 */
		moveTempPosition = static_cast<Vector2Float>(enableMoveTilcoord);
	}

	/* 벽에 부딪치고 이동이 대각 이동인 경우 축을 분리해서 이동한다.(슬라이딩효과) */
	if (CheckMoveResultType::BlockWall == checkResult &&
		AxisAdd.x != 0 && AxisAdd.y != 0)
	{
		//실제 이동한 만큼은 빼고 남은 이동거리를 구한다.
		float remainMoveDistance = moveDistance - (moveTempPosition - prevPosition).LengthSqrt();

		/* 남은 이동거리가 있으면 축 분리해서 막혀있지 않은쪽으로 움직인다. */
		if (remainMoveDistance > 0.f)
		{
			char szTmp[256] = { 0 };
			sprintf_s(szTmp, "Block Wall, xAxisAdd[%d], yAxisAdd[%d], MoveDistance[%f], remainDistance[%f]\n", AxisAdd.x, AxisAdd.y, moveDistance, remainMoveDistance);
			OutputDebugStringA(szTmp);

			//moveDirection * 

			//enableMoveTilcoord + ;
		}


		//TODO : 위 과정에서 실제 이동한 만큼은 빼고 나머지 델타를 가지고 이동시킨다.



	}
}
	}



	//void MovementComponent::UpdateMovement()
	//{
	//	/* 이동방향 벡터의 크기가 있을때만 작동 */
	//	if (lastMoveDirection == Vector2Int::Zero)
	//	{
	//		return;
	//	}

	//	std::shared_ptr<Actor> ownerActor = GetOwner();
	//	assert(ownerActor && "ownerActor is invalid");

	//	/* 현재 위치(Local 기준) */
	//	const Vector2Int currentPosition = ownerActor->GetPosition();

	//	/* 이동할 새로운 위치(Local 기준) */
	//	const Vector2Int newPosition = currentPosition + lastMoveDirection;

	//	/* 이동이 실제 가능한지 점검 필요 */
	//	std::shared_ptr<Level> level = ownerActor->GetOwner();
	//	assert(level && "level is invalid");
	//	if (level->CanNextMove(ownerActor, newPosition))
	//	{
	//		/* 이동할 새로운 위치 지정 */
	//		ownerActor->SetPosition(newPosition);
	//	}
	//	else if((lastMoveDirection.x != 0) && (lastMoveDirection.y != 0))
	//	{
	//		/* 대각 이동인 상태에서 이동이 막혔던 경우 막혀있지 않은 한쪽 축 방향으로 이동한다. */
	//		const Vector2Int newXPosition = currentPosition + Vector2Int(lastMoveDirection.x, 0);
	//		const Vector2Int newYPosition = currentPosition + Vector2Int(0, lastMoveDirection.y);
	//		const bool canMoveXPos = level->CanNextMove(ownerActor, newXPosition);
	//		const bool canMoveYPos = level->CanNextMove(ownerActor, newYPosition);

	//		if (canMoveXPos && !canMoveYPos)
	//		{
	//			ownerActor->SetPosition(newXPosition);
	//		}
	//		else if (!canMoveXPos && canMoveYPos)
	//		{
	//			ownerActor->SetPosition(newYPosition);
	//		}
	//		else if (canMoveXPos && canMoveYPos)
	//		{
	//			/* 대각이동은 막혔으나 x, y 개별축으로는 이동이 가능한 상태 */
	//			/* 이 경우 x축으로 이동 */
	//			ownerActor->SetPosition(newXPosition);
	//		}
	//	}

	//	/* 이동 방향 벡터 초기화 */
	//	lastMoveDirection = Vector2Int::Zero;
	//}
}