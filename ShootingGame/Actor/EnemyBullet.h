#pragma once

#include "Actor/Actor.h"

class EnemyBullet : public Craft::Actor
{
	//RTTI 등록
	TYPE_DECLARATIONS(EnemyBullet, Actor)

public:
	EnemyBullet(const Craft::Vector2& position, float moveSpeed = 15.f);
	~EnemyBullet() = default;

private:
	//Tick 이벤트
	virtual void Tick(float deltaTime) override;

private:
	//이동 처리를 위한 변수.
	float moveSpeed = 0.f;

	//y 이동 위치 처리를 위한 변수.
	float yPosition = 0.f;


};

