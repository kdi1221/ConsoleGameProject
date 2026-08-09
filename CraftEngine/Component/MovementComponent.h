#pragma once

#include "Component.h"
#include "Core/Core.h"
#include "Util/Timer.h"
#include "Math/Vector2Int.h"

namespace Craft
{
	/* 이동 컴포넌트(그리드 기반 8방향 이동) */
	class CRAFT_API MovementComponent : public Component
	{
		TYPE_DECLARATIONS(MovementComponent, Component)

	public:
		MovementComponent(float inMoveDelay);
		virtual ~MovementComponent() = default;

	private:
		virtual void Tick(float deltaTime) override;

	public:
		void SetLastMoveDirection(const Vector2Int& moveDirection);

	private:
		void UpdateMovement();

	private:
		// 이동 제어 타이머.
		Timer moveTimer;

		/* 직전 프레임에 저장된 이동 방향 */
		Vector2Int lastMoveDirection = Vector2Int::Zero;
	};
}


