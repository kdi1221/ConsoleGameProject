#pragma once

#include <Core/Core.h>
#include "Component.h"
#include <Math/Vector2Float.h>
#include <Math/Vector2Int.h>
#include <vector>
#include <functional>

namespace Craft
{
	/* 네비게이션 시스템 기반 이동 */
	class CRAFT_API NavMovementComponent : public Component
	{
		TYPE_DECLARATIONS(NavMovementComponent, Component)
	
	public:
		using OnMoveFinishType = std::function<void()>;
		using OnMoveAbort = std::function<void()>;

	public:
		NavMovementComponent(float moveSpeed);
		virtual ~NavMovementComponent() = default;

	private:
		virtual void Tick(float deltaTime) override;

	public:
		/* 이동 완료되었을때 호출되는 콜백 지정 */
		void SetMoveFinishCallback(OnMoveFinishType callback);

		/* 이동 중단되었을때 호출되는 콜백 지정 */
		void SetMoveAbortCallback(OnMoveAbort callback);

		/* 이동속도 설정 */
		void SetMoveSpeed(float newMoveSpeed);

		/* 지정한 위치로 이동 시작 */
		bool StartMove(const Vector2Int& destination);

		/* 이동 중지 */
		void StopMove();

		/* 경로별 콜백 호출 */
		void Foreach_Path(std::function<void(const Vector2Int&)> callback);

	private:
		/* Owner 위치 설정 */
		void SetOwnerPosition(const Vector2Int& inPosition);

		/* 다음 경로 지정 */
		void ToNextPosition();

		/* 실제 이동 실행 */
		void ProcessPathMove(float deltaTime);

		/* 다음 위치 이동 가능한지 체크 */
		bool IsValidNextMove() const;

	private:
		/* 현재 이동 진행중인지 여부 */
		bool isMoveProcess = false;

		/* 이동 속도 */
		float moveSpeed = 0.f;

		/* 이동 경로 좌표*/
		std::vector<Vector2Int> movePaths;

		/* 현재 지정된 이동 경로 위치 */
		int nextPathIndex = -1;

		/* 현재 위치 값(실수형) */
		Vector2Float currentPositionTemp = Vector2Float::Zero;

		/* 이동 완료되었을 때 호출 */
		OnMoveFinishType onMoveFinish;

		/* 이동 도중 이동이 불가능한 상황 때 호출 */
		OnMoveAbort onMoveAbort;
	};
}



