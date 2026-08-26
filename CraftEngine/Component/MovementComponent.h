#pragma once

#include "Component.h"
#include "Core/Core.h"
#include "Math/Vector2Float.h"
#include <string>
#include <unordered_map>

namespace Craft
{
	enum class eDirection : unsigned int
	{
		None = 0,
		Left = 1 << 0,
		Top = 1 << 1,
		Right = 1 << 2,
		Bottom = 1 << 3,
		LeftTop = Left | Top,
		RightTop = Right | Top,
		LeftBottom = Left | Bottom,
		RightBottom = Right | Bottom,
	};

	//eDirection의 비트플래그동작을 위한 연산자 오버로딩
	inline constexpr eDirection operator|(eDirection lhs, eDirection rhs)
	{
		return static_cast<eDirection>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
	}

	inline constexpr eDirection operator&(eDirection lhs, eDirection rhs)
	{
		return static_cast<eDirection>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
	}

	inline bool IsHorizontal(const eDirection direction)
	{
		return eDirection::None != (direction & (eDirection::Left | eDirection::Right));
	}

	inline bool IsVertical(const eDirection direction)
	{
		return eDirection::None != (direction & (eDirection::Top | eDirection::Bottom));
	}

	inline bool IsDiagonal(const eDirection direction)
	{
		return IsHorizontal(direction) && IsVertical(direction);
	}

	inline std::wstring ToString(const eDirection direction)
	{
		switch (direction)
		{
		case eDirection::Left:
			return L"Left";

		case eDirection::Top:
			return L"Top";

		case eDirection::Right:
			return L"Right";

		case eDirection::Bottom:
			return L"Bottom";

		case eDirection::LeftTop:
			return L"LeftTop";

		case eDirection::RightTop:
			return L"RightTop";

		case eDirection::LeftBottom:
			return L"LeftBottom";

		case eDirection::RightBottom:
			return L"RightBottom";
		}

		return L"None";
	}

	/* 대각선 이동에 대한 기본 빗변 길이 */
	constexpr float DIAGONAL_VALUE = 0.7071f;

	/* 각 이동 방향에 대한 실제 벡터 값들 */
	extern const CRAFT_API std::unordered_map<eDirection, Vector2Float> MOVE_DIRECTION;


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


