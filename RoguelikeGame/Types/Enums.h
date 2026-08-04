#pragma once

//타일 종류
enum class eTileCategory
{
	//기본값
	None = -1,

	//지면
	Ground = 0,

	//벽
	Wall
};

//방의 4분면 정의
enum class eRoomSides
{
	//왼쪽 면
	Left = 0,

	//위쪽 면
	Top,

	//오른쪽 면
	Right,

	//아래쪽 면
	Bottom,

	MaxNum
};

//두 방의 면이 마주보는 방향
enum class eRoomBetweenFace : unsigned int
{
	None = 0,

	//방의 왼쪽
	Left = 1 << 0,

	//방의 오른쪽
	Right = 1 << 1,

	//방의 위쪽
	Up = 1 << 2,

	//방의 아래쪽
	Down = 1 << 3,

	Left_Right = Left | Right,
	Up_Down = Up | Down,
	Left_Up = Left | Up,
	Left_Down = Left | Down,
	Right_Up = Right | Up,
	Right_Down = Right | Down
};

//eRoomBetweenFace의 비트플래그동작을 위한 연산자 오버로딩
inline constexpr eRoomBetweenFace operator|(eRoomBetweenFace lhs, eRoomBetweenFace rhs)
{
	return static_cast<eRoomBetweenFace>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

inline constexpr eRoomBetweenFace operator&(eRoomBetweenFace lhs, eRoomBetweenFace rhs)
{
	return static_cast<eRoomBetweenFace>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
}