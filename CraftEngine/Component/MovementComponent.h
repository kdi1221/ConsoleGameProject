#pragma once

#include "Component.h"
#include "Core/Core.h"
#include "Math/Vector2Float.h"
//#include "Util/Timer.h"
//#include "Math/Vector2Int.h"

namespace Craft
{
	/* 이동 컴포넌트(그리드 기반 8방향 이동) */
	class CRAFT_API MovementComponent : public Component
	{
		TYPE_DECLARATIONS(MovementComponent, Component)

	public:
		//MovementComponent(float inMoveDelay);
		MovementComponent(float inMoveSpeed);
		virtual ~MovementComponent() = default;

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;

	public:
		void SetLastMoveDirection(const Vector2Float& moveDirection);
		//void SetLastMoveDirection(const Vector2Int& moveDirection);

	private:
		void UpdateMovement(const float deltaTime);
		//void UpdateMovement();

	private:
		/* 현재 Owner의 위치(float) */
		Vector2Float currentWorldPosition = Vector2Float::Zero;

		/* 직전 프레임에 저장된 이동 방향 */
		Vector2Float lastMoveDirection = Vector2Float::Zero;

		/* 이동 속도 */
		float moveSpeed = 0.f;

		// 이동 제어 타이머.
		//Timer moveTimer;

		/* 직전 프레임에 저장된 이동 방향 */
		//Vector2Int lastMoveDirection = Vector2Int::Zero;
	};
}


