#pragma once

#include "Actor/Actor.h"

class PlayerBullet : public Craft::Actor
{
	//RTTI 등록
	TYPE_DECLARATIONS(PlayerBullet, Actor)

public:
	PlayerBullet(const Craft::Vector2& position);
	~PlayerBullet() = default;

public:
	virtual void Tick(float deltaTime) override;

private:
	//초당 이동 속도
	float moveSpeed = 30.f;

	//위치 업데이트를 할 때 소수점 처리를 위한 변수.
	float yPosition = 0.f;
};

