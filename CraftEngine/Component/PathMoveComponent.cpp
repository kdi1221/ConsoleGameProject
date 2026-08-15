#include "PathMoveComponent.h"
#include <Navigation/NavigationBase.h>
#include <Engine/Engine.h>
#include <Actor/Actor.h>
#include <cassert>

namespace Craft
{
	PathMoveComponent::PathMoveComponent(float moveDelay, bool checkEnableMove)
		:isMoveCheckEnable(checkEnableMove)
	{
		// 경로 이동 타이머 딜레이 지정
		timerMovePath.SetTargetTime(moveDelay);
	}

	void PathMoveComponent::SetMoveFinishCallback(OnMoveFinishType callback)
	{
		onMoveFinish = callback;
	}

	void PathMoveComponent::SetMoveAbortCallback(OnMoveAbort callback)
	{
		onMoveAbort = callback;
	}

	void PathMoveComponent::StartMove(std::vector<Vector2Int>&& srcPaths)
	{
		/* 경로 설정 */
		movePaths = std::move(srcPaths);

		/* 경로가 비어있으면 진행 x */
		if(movePaths.empty())
		{ 
			return;
		}

		/* 이동 시작 지정 */
		isMoveProcess = true;

		/* 첫 경로 지정(현재 위치) */
		iterCurrentPath = movePaths.begin();

		/* 이동할 다음 위치 인덱스 지정 */
		ToNextPosition();
	}

	void PathMoveComponent::StopPathMove()
	{
		movePaths.clear();
		timerMovePath.Reset();
		isMoveProcess = false;
		iterCurrentPath = movePaths.end();
	}

	void PathMoveComponent::Foreach_Path(std::function<void(const Vector2Int&)> callback)
	{
		for (const auto& path : movePaths)
		{
			callback(path);
		}
	}

	void PathMoveComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		if (isMoveProcess)
		{
			timerMovePath.Tick(deltaTime);
			if (timerMovePath.IsTimeOut())
			{
				ProcessCurrentPathMove();
			}
		}
	}

	void PathMoveComponent::ToNextPosition()
	{
		//다음 타이머에 이동할 경로 지정
		++iterCurrentPath;

		//더이상 이동 경로 없으면 종료
		if (iterCurrentPath == movePaths.end())
		{
			StopPathMove();

			if (onMoveFinish)
			{
				onMoveFinish();
			}
		}
		else
		{
			/* 타이머 초기화 */
			timerMovePath.Reset();
		}
	}

	void PathMoveComponent::ProcessCurrentPathMove()
	{
		const Vector2Int& currentPathPos = *iterCurrentPath;

		if (isMoveCheckEnable)
		{
			const NavigationBase& navigationSystem = Engine::Get().GetNavigationSystem<NavigationBase>();
			if (!navigationSystem.CanNextMove(GetOwner(), currentPathPos))
			{
				/* 현재 Path이 이동이 불가능할때는 이동 종료 */
				StopPathMove();

				if (onMoveAbort)
				{
					onMoveAbort();
				}

				return;
			}
		}

		/* 이동 딜레이마다 해당 경로에 위치 */
		SetOwnerPosition(currentPathPos);

		/* 다음 이동 위치 지정(경로가 없으면 종료 알림) */
		ToNextPosition();
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