#pragma once

#include "Level/Level.h"

class GameLevel : public Craft::Level
{
public:
	// 레벨 초기화 함수
	virtual void OnInitialized() override;

	//Draw 이벤트 함수.
	virtual void Draw() override;

private:
	// 맵 로드 함수(로드할 맵의 이름을 전달).
	void LoadMap(const std::string& filename);

private:
	// 목표 점수 - 클리어 조건.
	int targetScore = 0;

	// 게임 클리어 여부를 나타내는 플래그.
	bool isGameClear = false;
};

