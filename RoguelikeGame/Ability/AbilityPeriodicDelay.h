#pragma once

#include "AbilityObject.h"
#include <Util/Timer.h>

//주기적으로 실행되는 Ability의 Base 클래스
class AbilityPeriodicDelay : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityPeriodicDelay, AbilityObject)

public:
	AbilityPeriodicDelay(ABILITY_ID_TYPE id, int level, float interval);
	virtual ~AbilityPeriodicDelay() = default;

private:
	virtual void Tick(float deltaTime);

public:
	/* 트리거 켜기 */
	virtual void TriggerOn();

	/* 트리거 끄기 */
	virtual void TriggerOff();

protected:
	/* 딜레이 타이머 동작 */
	virtual void OnIntervalTrigger() = 0;

private:
	/* 딜레이 타이머*/
	Timer timerInterval;
};

