#include "Timer.h"

Timer::Timer(float targetTime)
	: elapsedTime(0.f)
	, targetTime(targetTime)
{

}

void Timer::Tick(float deltaTime)
{
	//경과 시간 업데이트
	elapsedTime += deltaTime;

	//경과 시간이 목표시간을 벗어나지 않도록 고정
	elapsedTime = elapsedTime >= targetTime ? targetTime : elapsedTime;
}

void Timer::Reset()
{
	//경과 시간 초기화
	elapsedTime = 0.f;
}

void Timer::ReserveNextTick()
{
	elapsedTime = targetTime;
}

void Timer::SetTargetTime(float newTargetTime)
{
	targetTime = newTargetTime;
}