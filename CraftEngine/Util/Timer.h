#pragma once

#include "Core/Core.h"

// 원하는 시간(단위: 초)을 설정해 시간이 모두 지났는지 확인이 가능한 타이머
class CRAFT_API Timer
{
public:
	Timer(float targetTime = 1.f);

	//타이머 업데이트 함수.
	void Tick(float deltaTime);

	//경과 시간 리셋 함수.
	void Reset();

	// 목표 시간 재설정 함수.
	void SetTargetTime(float newTargetTime);

	// 설정한 시간만큼 흘렀는지 확인하는 함수.
	inline bool IsTimeOut() const { return elapsedTime >= targetTime; }

private:
	// 경과 시간 계산용 변수.
	float elapsedTime = 0.f;

	// 타이머 목표 시간.
	float targetTime = 0.f;

};

