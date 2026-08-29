#pragma once

#include "Component.h"
#include "Core/Core.h"
#include "Math/Vector2Float.h"
#include "Defines/Enums.h"
#include <string>
#include <unordered_map>

namespace Craft
{
	/* 이동 컴포넌트(그리드 기반 8방향 이동) */
	class CRAFT_API MovementComponent : public Component
	{
		TYPE_DECLARATIONS(MovementComponent, Component)

	public:
		MovementComponent(float inMoveSpeed);
		virtual ~MovementComponent() = default;

	private:
		virtual void Tick(float deltaTime) override;

	public:
		void SetLastMoveInputDireciton(const eDirection newDirection);

	public:
		inline eDirection GetCurrentMoveDirection() const { return currentMoveDirection; }

	private:
		/* 이동 델타 누산값 초기화 */
		void ResetAccumulatorMoveDelta();

		/* 매 프레임 이동 실행 */
		void UpdateMovement(const float deltaTime);

	private:
		/* 이동 속도 */
		float moveSpeed = 0.f;

		/* 현재 이동 방향 */
		eDirection currentMoveDirection = eDirection::None;

		/* 이동델타 누산값 */
		Vector2Float accumulatorMoveDelta = Vector2Float::Zero;
	};
}


