#pragma once

#include "Math/Vector2.h"

// 플레이어가 이동할 위치를 판단해주는 인터페이스.(=> 순수 가상함수를 가지는 클래스)
class ICanPlayerMove
{
public:
	//플레이어가 이동 가능한지 여부를 문의할 때 사용.
	virtual bool CanMove(const Craft::Vector2& playerPosition, const Craft::Vector2& nextPosition) = 0;
};