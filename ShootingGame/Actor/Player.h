#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

class Player : public Craft::Actor
{
	//발사 모드(단발/연사).
	enum class FireMode
	{
		None = -1,
		OneShot,
		Repeat
	};

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

	//탄약 발사 함수.
	void Fire();

	//연속 발사 함수.
	void FireInterval();

	//발사 가능 여부 확인 함수.
	inline bool CanShoot() const { return fireTimer.IsTimeOut(); }

private:
	//좌우 이동을 처리할 때 사용할 변수.
	float xPosition = 0.0f;

	// 이동 속도 변수.
	float moveSpeed = 70.f;

	// 발사 모드.
	FireMode fireMode = FireMode::None;

	// 연사 제어 타이머.
	Timer fireTimer;

	// 연사 간격(단위 : 초)
	float fireInterval = 0.1f;
};

