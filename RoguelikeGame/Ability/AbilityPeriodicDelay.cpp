#include "AbilityPeriodicDelay.h"

AbilityPeriodicDelay::AbilityPeriodicDelay(float interval)
{
	timerInterval.SetTargetTime(interval);
}

void AbilityPeriodicDelay::Tick(float deltaTime)
{
	if (isTimerActive)
	{
		timerInterval.Tick(deltaTime);

		if (timerInterval.IsTimeOut())
		{
			OnIntervalTrigger();

			timerInterval.Reset();
		}
	}
}

void AbilityPeriodicDelay::TriggerOn()
{
	SetEnableTimer(true);
}

void AbilityPeriodicDelay::TriggerOff()
{
	SetEnableTimer(false);
}

void AbilityPeriodicDelay::SetEnableTimer(bool enable)
{
	if (enable == isTimerActive)
	{
		return;
	}

	isTimerActive = enable;

	/* 타이머 리셋 */
	timerInterval.Reset();
}
