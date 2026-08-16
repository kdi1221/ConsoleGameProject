#include "AbilityPeriodicDelay.h"

AbilityPeriodicDelay::AbilityPeriodicDelay(ABILITY_ID_TYPE id, int level, float interval)
	:super(id, level)
{
	timerInterval.SetTargetTime(interval);
}

void AbilityPeriodicDelay::Tick(float deltaTime)
{
	timerInterval.Tick(deltaTime);

	if (timerInterval.IsTimeOut())
	{
		OnIntervalTrigger();

		timerInterval.Reset();
	}
}

void AbilityPeriodicDelay::TriggerOn()
{
	super::TriggerOn();

	timerInterval.Reset();
}

void AbilityPeriodicDelay::TriggerOff()
{
	super::TriggerOff();

	timerInterval.Reset();
}
