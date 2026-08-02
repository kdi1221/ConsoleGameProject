#pragma once

#include "Actor/Actor.h"

//플레이어에 붙어서 총알 발사 위치를 제공하는 액터.
class PlayerGun : public Craft::Actor
{
	// 타입 설정.
	TYPE_DECLARATIONS(PlayerGun, Actor)

public:
	PlayerGun(const Craft::Vector2& localPosition);
	~PlayerGun() = default;
	
public:
	//총알이 실제로 생성될 월드 위치 반환 함수.
	Craft::Vector2 GetFirePosition() const;
};

