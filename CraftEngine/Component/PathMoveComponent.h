#pragma once

#include <Core/Core.h>
#include "Component.h"
#include <Util/Timer.h>
#include <Math/Vector2Int.h>
#include <vector>
#include <functional>

namespace Craft
{
	class CRAFT_API PathMoveComponent : public Component
	{
		TYPE_DECLARATIONS(PathMoveComponent, Component)

	public:
		using OnMoveFinishType = std::function<void()>;
		using OnMoveAbort = std::function<void()>;

	public:
		PathMoveComponent(float moveDelay, bool checkEnableMove);
		virtual ~PathMoveComponent() = default;

	public:
		void SetMoveFinishCallback(OnMoveFinishType callback);
		void SetMoveAbortCallback(OnMoveAbort callback);
		void StartMove(std::vector<Vector2Int>&& srcPaths);
		void StopPathMove();

	private:
		virtual void Tick(float deltaTime) override;

	private:
		void ToNextPosition();
		void ProcessCurrentPathMove();
		void SetOwnerPosition(const Vector2Int& inPosition);

	private:
		Vector2Int GetOwnerPosition() const;

	private:
		/* 경로 이동 중 이동 가능여부 검사 */
		bool isMoveCheckEnable = false;

		/* 이동 완료되었을 때 호출 */
		OnMoveFinishType onMoveFinish;

		/* 이동 도중 이동이 불가능한 상황 때 호출 */
		OnMoveAbort onMoveAbort;

		/* 경로 이동 딜레이 타이머 */
		Timer timerMovePath;

	private:
		/* 현재 이동 진행중인지 여부 */
		bool isMoveProcess = false;

		/* 현재 지정된 이동 경로 위치 */
		std::vector<Vector2Int>::iterator iterCurrentPath;

		/* Projectile의 이동 경로 */
		std::vector<Vector2Int> movePaths;
	};
}


