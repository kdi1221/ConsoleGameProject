#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

class Enemy : public Craft::Actor
{
	// RTTI 등록
	TYPE_DECLARATIONS(Enemy, Actor)

	//이동 방향 열거형
	enum class MoveDirection
	{
		None = -1,
		Left,
		Right
	};

public:
	Enemy(const std::string& image = "(o0o)", int yPosition = 5);
	~Enemy() = default;

private:
	virtual void Tick(float deltaTime) override;

private:
	//이동 방향
	MoveDirection direction = MoveDirection::None;

	//좌우 이동 처리를 위한 변수.
	float xPosition = 0.f;
	float moveSpeed = 5.f;

	//발사 타이머.
	Timer timer;
};

