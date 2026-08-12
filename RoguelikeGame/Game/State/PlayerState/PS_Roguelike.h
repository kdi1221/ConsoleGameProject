#pragma once

#include "GameState/PlayerState/PlayerState.h"

/* 로그라이크 게임내에서 플레이어의 상태 값 저장 */
class PS_Roguelike : public Craft::PlayerState
{
	TYPE_DECLARATIONS(PS_Roguelike, PlayerState)

public:
	PS_Roguelike();
	virtual ~PS_Roguelike();
};

