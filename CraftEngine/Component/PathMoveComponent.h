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

	public:
		PathMoveComponent(float moveDelay, const OnMoveFinishType& moveFinishCallback);
		virtual ~PathMoveComponent() = default;

	public:
		void StartMoveToPosition(const Vector2Int& inDestinationPos);

	private:
		virtual void Tick(float deltaTime) override;

	private:
		void ToNextPosition();
		void SetOwnerPosition(const Vector2Int& inPosition);

	private:
		Vector2Int GetOwnerPosition() const;

	private:
		/* 경로 이동 딜레이 타이머 */
		Timer timerMovePath;

		/* 현재 지정된 이동 경로 위치 */
		std::vector<Vector2Int>::iterator iterCurrentPath;

		/* Projectile의 이동 경로 */
		std::vector<Vector2Int> movePaths;

		/* 이동 완료되었을 때 호출 */
		OnMoveFinishType onMoveFinish;
	};
}


