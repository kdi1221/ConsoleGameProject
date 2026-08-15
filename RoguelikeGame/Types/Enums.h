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
enum class eRoomFacingFlags : unsigned int
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

	Right_Left = Left | Right,
	Down_Up = Up | Down,
	/*Up_Left = Left | Up,
	Down_Left = Left | Down,
	Up_Right = Right | Up,
	Down_Right = Right | Down*/
};

//eRoomBetweenFace의 비트플래그동작을 위한 연산자 오버로딩
inline constexpr eRoomFacingFlags operator|(eRoomFacingFlags lhs, eRoomFacingFlags rhs)
{
	return static_cast<eRoomFacingFlags>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}

inline constexpr eRoomFacingFlags operator&(eRoomFacingFlags lhs, eRoomFacingFlags rhs)
{
	return static_cast<eRoomFacingFlags>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
}

//생성된 방 종류
enum class eRoomType
{
	None = 0,
	
	//시작지점
	Start,

	//다음 레벨
	NextLevel,

	//출구(최종 목표)
	Exit,

	//보물창고(아이템)
	Treasure,

	//전투
	Battle
};

//오브젝트 랜더링시 그릴 순서
enum class eRenderSortingOrder : int
{
	//타일
	Tile = 0,

	//필드 내 존재하는 아이템
	FieldItem,

	//탄환
	Projectile,

	//맵 상의 오브젝트
	MapObject,

	//폰
	Pawn,

	//UI
	UI
};

//팀 ID
enum class eTeamID
{
	None = 0,
	Player,
	NPC
};