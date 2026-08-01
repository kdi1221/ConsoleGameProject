#pragma once

#include "Actor/Actor.h"

class Player : public Craft::Actor
{
	// RTTI 등록.
	TYPE_DECLARATIONS(Player, Actor)

public:
	Player();

private:
	//이벤트 함수 오버라이드.
	virtual void Tick(float deltaTime) override;

private:
	//이동 처리 함수(내부에서 사용).
	void Move(float direction, float deltaTime);

private:
	//좌우 이동을 처리할 때 사용할 변수.
	float xPosition = 0.0f;

	// 이동 속도 변수.
	float moveSpeed = 70.f;
};

