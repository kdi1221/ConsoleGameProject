#pragma once

#include "Component.h"
#include "Math/Vector2Float.h"

namespace Craft
{
	/* 발사체를 이동시키는 컴포넌트 */
	class CRAFT_API ProjectileMoveComponent : public Component
	{
		TYPE_DECLARATIONS(ProjectileMoveComponent, Component)

	public:
		ProjectileMoveComponent(float moveSpeed);
		virtual ~ProjectileMoveComponent() = default;

	private:
		virtual void Tick(float deltaTime) override;

	public:
		/* 이동방향 지정 */
		void SetMoveDirection(const Vector2Float& newDirection);

	public:
		inline const Vector2Float& GetMoveDirection() const { return currentMoveDirection; }

	private:
		/* 이동 델타 누산값 초기화 */
		void ResetAccumulatorMoveDelta();

		/* 매 프레임 이동 실행 */
		void UpdateMovement(const float deltaTime);

	private:
		/* 이동 속도 */
		float moveSpeed = 0.f;

		/* 현재 이동 방향 */
		Vector2Float currentMoveDirection = Vector2Float::Zero;

		/* 이동델타 누산값 */
		Vector2Float accumulatorMoveDelta = Vector2Float::Zero;
		
	};
}


