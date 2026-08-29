#pragma once

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

	inline const wchar_t* ToString(const eDirection direction)
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
}