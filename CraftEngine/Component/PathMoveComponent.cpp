#include "PathMoveComponent.h"
#include <Actor/Actor.h>
#include <cassert>
#include <StaticLibrary/StaticFunctionLibrary.h>

namespace Craft
{
	PathMoveComponent::PathMoveComponent(float moveDelay, const OnMoveFinishType& moveFinishCallback)
		:onMoveFinish(moveFinishCallback)
	{
		// 경로 이동 타이머 딜레이 지정
		timerMovePath.SetTargetTime(moveDelay);
	}

	void PathMoveComponent::StartMoveToPosition(const Vector2Int& inDestinationPos)
	{
		const Vector2Int startPostion = GetOwnerPosition();
		
		/* 브레젠험 알고리즘, 시작 지점과 끝 지점을 잇는 선 경로를 가져옴 */
		StaticFunctionLibrary::GetBresenhamPath(startPostion, inDestinationPos, movePaths);

		/* 첫 경로 지정(현재 위치) */
		iterCurrentPath = movePaths.begin();

		/* 이동할 다음 위치 인덱스 지정 */
		ToNextPosition();
	}

	void PathMoveComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		timerMovePath.Tick(deltaTime);
		if (timerMovePath.IsTimeOut())
		{
			/* 이동 딜레이마다 해당 경로에 위치 */
			SetOwnerPosition(*iterCurrentPath);

			/* 다음 이동 위치 지정(경로가 없으면 Destroy) */
			ToNextPosition();
		}
	}

	void PathMoveComponent::ToNextPosition()
	{
		//다음 타이머에 이동할 경로 지정
		++iterCurrentPath;

		//더이상 이동 경로 없으면 종료
		if (iterCurrentPath == movePaths.end())
		{
			onMoveFinish();
		}
		else
		{
			/* 타이머 초기화 */
			timerMovePath.Reset();
		}
	}

	void PathMoveComponent::SetOwnerPosition(const Vector2Int& inPosition)
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "Invalid ownerActor");

		ownerActor->SetPosition(inPosition);
	}

	Vector2Int PathMoveComponent::GetOwnerPosition() const
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "Invalid ownerActor");

		return ownerActor->GetWorldPosition();
	}
}