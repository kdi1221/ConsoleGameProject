#pragma once

#include "AbilityObject.h"
#include <Util/Timer.h>

//주기적으로 실행되는 Ability의 Base 클래스
class AbilityPeriodicDelay : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityPeriodicDelay, AbilityObject)

public:
	AbilityPeriodicDelay(float interval);
	virtual ~AbilityPeriodicDelay() = default;

private:
	virtual void Tick(float deltaTime);

public:
	/* 트리거 켜기 */
	virtual void TriggerOn();

	/* 트리거 끄기 */
	virtual void TriggerOff();

private:
	/* 타이머 플래그 설정 */
	void SetEnableTimer(bool enable);

protected:
	/* 딜레이 타이머 동작 */
	virtual void OnIntervalTrigger() = 0;

private:
	/* 타이머 동작 여부 */
	bool isTimerActive = false;

	/* 딜레이 타이머*/
	Timer timerInterval;
};

