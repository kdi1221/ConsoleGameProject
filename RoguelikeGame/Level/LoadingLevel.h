#pragma once

#include "Level/Level.h"
#include <vector>
#include <Util/Timer.h>
#include <thread>
#include <atomic>

/* 로딩 레벨 */
class LoadingLevel : public Craft::Level
{
	TYPE_DECLARATIONS(LoadingLevel, Level)

private:
	static const std::wstring loadingAsciiArts[];

public:
	LoadingLevel();
	virtual ~LoadingLevel();

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void LoadResource();
	void LoadSound();

private:
	int loadingSubStrOffset = 12;
	Timer timerSubStrOffsetUpdate;
	std::thread worker;
	std::atomic_bool loadComplete = false;
};

