#include "NavMovementComponent.h"
#include "Navigation/NavigationBase.h"
#include "Engine/Engine.h"
#include "Actor/Actor.h"
#include <cassert>

namespace Craft
{
	NavMovementComponent::NavMovementComponent(float moveSpeed)
		:moveSpeed(moveSpeed)
	{

	}

	void NavMovementComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		if (isMoveProcess)
		{
			ProcessPathMove(deltaTime);
		}
	}

	void NavMovementComponent::SetMoveFinishCallback(OnMoveFinishType callback)
	{
		onMoveFinish = callback;
	}

	void NavMovementComponent::SetMoveAbortCallback(OnMoveAbort callback)
	{
		onMoveAbort = callback;
	}

	void NavMovementComponent::SetMoveSpeed(float newMoveSpeed)
	{
		moveSpeed = newMoveSpeed;
	}

	bool NavMovementComponent::StartMove(const Vector2Int& destination)
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "Invalid ownerActor");

		const Vector2Int currentPosition = ownerActor->GetWorldPosition();

		// 현재위치와 같은 위치면 이동하지 않음
		if (currentPosition == destination)
		{
			return false;
		}

		//네비게이션 시스템을 통해 경로 찾기
		const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();
		const bool findPathResult = navigationSystem.FindPath(ownerActor, currentPosition, destination, movePaths);

		if(!findPathResult || movePaths.empty())
		{
			//경로를 찾지 못했거나 경로가 비어있으면 이동 중단
			return false;
		}

		/* 이동 시작 지정 */
		isMoveProcess = true;

		/* 시작 위치 저장 */
		currentPositionTemp = static_cast<Vector2Float>(currentPosition);

		/* 첫 경로 지정(현재 위치) */
		nextPathIndex = 0;

		/* 이동할 다음 위치 인덱스 지정 */
		ToNextPosition();

		return true;
	}

	void NavMovementComponent::StopMove()
	{
		/* 이동 경로 정보 초기화 */
		movePaths.clear();
		nextPathIndex = -1;
		currentPositionTemp = Vector2Float::Zero;
		isMoveProcess = false;
	}

	void NavMovementComponent::Foreach_Path(std::function<void(const Vector2Int&)> callback)
	{
		if (!isMoveProcess)
		{
			return;
		}

		for(int drawPathIndex = nextPathIndex; 
			drawPathIndex < static_cast<int>(movePaths.size()); 
			++drawPathIndex)
		{
			callback(movePaths[drawPathIndex]);
		}
	}

	void NavMovementComponent::SetOwnerPosition(const Vector2Int& inPosition)
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "Invalid ownerActor");

		ownerActor->SetPosition(inPosition);
	}

	void NavMovementComponent::ToNextPosition()
	{
		//다음 번 이동 경로 지정 
		++nextPathIndex;

		if (nextPathIndex >= static_cast<int>(movePaths.size()))
		{
			//더이상 이동 경로 없으면 이동 종료
			StopMove();

			if (onMoveFinish)
			{
				onMoveFinish();
			}
		}
		else
		{
			/* 다음 위치가 이동 불가능한 경우 이동 중단 및 중단 이벤트 호출 */
			if (!IsValidNextMove())
			{
				StopMove();

				if (onMoveAbort)
				{
					onMoveAbort();
				}
			}
		}
	}

	void NavMovementComponent::ProcessPathMove(float deltaTime)
	{
		/* 다음 위치가 이동 불가능한 경우 이동 중단 및 중단 이벤트 호출 */
		if (!IsValidNextMove())
		{
			StopMove();

			if (onMoveAbort)
			{
				onMoveAbort();
			}

			return;
		}

		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "Invalid ownerActor");

		const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();

		/* 현재 프레임에서의 이동 델타 값 */
		float moveDelta =  deltaTime * moveSpeed;

		/* Length sqrt연산 제외를 위해 제곱 */
		//const float sqrMoveDelta = moveDelta * moveDelta;

		/* 이동 델타값과, 이동할 경로가 남아있는동안 반복 */
		while (moveDelta > 0.f && isMoveProcess)
		{
			assert(nextPathIndex >= 0 && nextPathIndex < static_cast<int>(movePaths.size()) && "Invalid currentPathIndex");

			/* 다음 이동 경로 */
			const Vector2Int& nextMovePosition = movePaths[nextPathIndex];

			/* 다음 이동 경로(실수형)*/
			const Vector2Float nextMovePositionFloat = static_cast<Vector2Float>(nextMovePosition);

			/* 현재 위치에서 다음 이동 경로까지의 방향 */
			Vector2Float toNextPositionDirection = nextMovePositionFloat - currentPositionTemp;

			/* 현재 위치에서 다음 이동 경로까지의 직선 길이(sqrt 제외) */
			const float toNextPositionDistance = toNextPositionDirection.LengthSqrt();

			if (moveDelta >= toNextPositionDistance)
			{
				/* Owner의 실제 위치 갱신 */
				SetOwnerPosition(nextMovePosition);

				/* 이동 델타 값 감소 */
				moveDelta -= toNextPositionDistance;

				/* 다음 이동 위치 지정(경로가 없거나 막힌 경우 종료 알림) */
				ToNextPosition();
			}
			else
			{
				/* 이동 델타 값이 다음 이동 경로까지의 길이보다 작으면 float형 임시 위치만 움직이고 종료한다. */
				toNextPositionDirection.Normalize();

				currentPositionTemp += toNextPositionDirection * moveDelta;

				moveDelta = 0.f;
			}

		}
	}

	bool NavMovementComponent::IsValidNextMove() const
	{
		/* 다음 경로 인덱스가 경로 최대 크기보다 큰 경우 예외 처리 */
		if (nextPathIndex >= static_cast<int>(movePaths.size()))
		{
			return false;
		}

		const Vector2Int& nextPathPos = movePaths[nextPathIndex];
		const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();
	
		//TODO : CanNextMove내에서 대각선 이동 체크 필요 
		return navigationSystem.CanNextMove(GetOwner(), nextPathPos);
	}
}


