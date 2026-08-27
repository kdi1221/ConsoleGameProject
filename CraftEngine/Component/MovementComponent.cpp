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
			//직전 이동방향과 현재 이동방향이 다르면 이동 델타 누적값 초기화
			ResetAccumulatorMoveDelta();
		}

		/* 이동 방향 변경 */
		currentMoveDirection = newDirection;
	}

	void MovementComponent::ResetAccumulatorMoveDelta()
	{
		accumulatorMoveDelta = Vector2Float::Zero;
	}

	void MovementComponent::UpdateMovement(const float deltaTime)
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "ownerActor is invalid");

		const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();

		/* 직전까지의 타일의 좌표 */
		const Vector2Int prevTilecoord = ownerActor->GetWorldPosition();

		/* 직전까지의 Delta */
		const Vector2Float prevDelta = accumulatorMoveDelta;

		auto movePosition = [&navigationSystem, this](std::shared_ptr<Actor> ownerActor, 
													const Vector2Float& moveDirection, 
													const float moveDistance)
			{
				/* 현재 프레임 이동 델타 */
				const Vector2Float moveDelta = moveDirection * moveDistance;

				/* 새로 누적된 이동 델타 */
				const Vector2Float newAccumulatorMoveDelta = accumulatorMoveDelta + moveDelta;

				/* 새로 누적된 이동 델타의 X, Y 축의 절대값이 1을 넘기지 않으면  */
				if (std::abs(newAccumulatorMoveDelta.x) < 1.f && abs(newAccumulatorMoveDelta.y) < 1.f)
				{
					/* 누적된 이동 델타값만 갱신하고 종료 */
					accumulatorMoveDelta = newAccumulatorMoveDelta;
					return CheckMoveResultType::Success;
				}
				else /* 새로 누적된 이동 델타의 X, Y축 중 하나의 절대값이 1을 넘기면 타일 이동 처리  */
				{
					/* 현재 타일위치에 가산할 타일 델타 값 */
					const Vector2Int addTileCoordDelta(static_cast<int>(newAccumulatorMoveDelta.x), 
														static_cast<int>(newAccumulatorMoveDelta.y));

					//-64.7152 => -64, 84.1234 => 84 | -64.7152 - -64 => -0.7152, 84.1234 - 84 => 0.1234
					const float remainXAccumulatorDelta = newAccumulatorMoveDelta.x - static_cast<float>(addTileCoordDelta.x);
					const float remainYAccumulatorDelta = newAccumulatorMoveDelta.y - static_cast<float>(addTileCoordDelta.y);

					/* 현재 위치한 타일의 좌표 */
					const Vector2Int prevTileCoord = ownerActor->GetWorldPosition();

					/* 이동할 타일 위치 좌표 */
					const Vector2Int nextTileCoord = prevTileCoord + addTileCoordDelta;
				
					/* 직선경로를 따라 충돌되기 직전의 이동가능한 위치를 가져온다. */
					Vector2Int enableMoveTilcoord = prevTileCoord;
					const CheckMoveResultType checkResult = navigationSystem.CheckEnableMoveToTargetPosition(ownerActor, nextTileCoord, enableMoveTilcoord);

					switch (checkResult)
					{
						/* 목적지까지 아무런 장애물 없이 도착 가능 */
					case CheckMoveResultType::Success:
						{
							/* 마지막으로 이동 가능한것으로 확인된 위치로 이동한다. */
							ownerActor->SetPosition(enableMoveTilcoord);

							/* 누적 이동 델타값을 소수점 부분들만 남긴다. */
							accumulatorMoveDelta.x = remainXAccumulatorDelta;
							accumulatorMoveDelta.y = remainYAccumulatorDelta;
						}
						break;

						/* 기타 충돌 발생해서 목적지까지 못간 상황 */
					default: 
						{
							if (prevTileCoord != enableMoveTilcoord)
							{
								/* 현재좌표와 충돌발생직전위치가 다르면 위치를 갱신하고 누적 이동값을 초기화한다. */
								ownerActor->SetPosition(enableMoveTilcoord);
								accumulatorMoveDelta = Vector2Float::Zero;
							}
						}
						break;
					}

					return checkResult;
				}
			};

		/* 이동값 계산(방향 * 속도 * 델타타임) */
		const Vector2Float& moveDirection = MOVE_DIRECTION.at(currentMoveDirection);
		const float moveDistance = moveSpeed * deltaTime;

		/* 처음 이동 시도 */
		const CheckMoveResultType moveResult = movePosition(ownerActor, moveDirection, moveDistance);

		/* For Debug.. */
		/*char szTmp[256] = { 0 };
		const Vector2Int currentPosition = ownerActor->GetPosition();
		sprintf_s(szTmp, "MoveDirection[%ls], Prev moveTempPosition[%d %d], current moveTempPosition[%d %d]\n", ToString(currentMoveDirection).c_str(), prevTilecoord.x, prevTilecoord.y, currentPosition.x, currentPosition.y);
		OutputDebugStringA(szTmp);

		sprintf_s(szTmp, "PrevDelta[%f, %f], CurrentDelta[%f %f]\n", prevDelta.x, prevDelta.y, accumulatorMoveDelta.x, accumulatorMoveDelta.y);
		OutputDebugStringA(szTmp);*/

		/* 목적지까지 이동 실패했고 대각이동이고 위치에 변화가 없다면 충돌상황이므로 축을 분리해서 이동한다(슬라이딩 효과) */
		const Vector2Int postMoveTileCoord = ownerActor->GetWorldPosition();
		if (moveResult != CheckMoveResultType::Success &&
			prevTilecoord == postMoveTileCoord && 
			IsDiagonal(currentMoveDirection))
		{
			const Vector2Int& addXDir = (eDirection::None != (currentMoveDirection & eDirection::Left)) ? Vector2Int::Left : Vector2Int::Right;
			const Vector2Int& addYDir = (eDirection::None != (currentMoveDirection & eDirection::Top)) ? Vector2Int::Up : Vector2Int::Down;
			const Vector2Int newXAxisTileCoord = postMoveTileCoord + addXDir;
			const Vector2Int newYAxisTileCoord = postMoveTileCoord + addYDir;

			const bool canMoveXAxis = navigationSystem.CanNextMove(ownerActor, newXAxisTileCoord);
			const bool canMoveYAXis = navigationSystem.CanNextMove(ownerActor, newYAxisTileCoord);

			if (canMoveXAxis && !canMoveYAXis)
			{
				const Vector2Float& xAxisDirection = (eDirection::None != (currentMoveDirection & eDirection::Left)) ? Vector2Float::Left : Vector2Float::Right;
				movePosition(ownerActor, xAxisDirection, moveDistance);
			}
			else if (!canMoveXAxis && canMoveYAXis)
			{
				const Vector2Float& yAxisDirection = (eDirection::None != (currentMoveDirection & eDirection::Top)) ? Vector2Float::Up : Vector2Float::Down;
				movePosition(ownerActor, yAxisDirection, moveDistance);
			}
			else if (canMoveXAxis && canMoveYAXis)
			{
				/* 대각이동은 막혔으나 x, y 개별축으로는 이동이 가능한 상태 */
				/* 이 경우 x축으로 이동 */
				const Vector2Float& xAxisDirection = (eDirection::None != (currentMoveDirection & eDirection::Left)) ? Vector2Float::Left : Vector2Float::Right;
				movePosition(ownerActor, xAxisDirection, moveDistance);
			}
		}
	}
}