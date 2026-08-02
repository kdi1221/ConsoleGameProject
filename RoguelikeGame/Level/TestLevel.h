#pragma once

#include "Level/Level.h"
#include <Memory>

class Tilemap;

//타일맵 테스트 및 개발용 레벨
class TestLevel : public Craft::Level
{
public:
	TestLevel();
	virtual ~TestLevel();

private:
	virtual void OnInitialized() override;
	virtual void Draw() override;

private:
	std::unique_ptr<Tilemap> tileMap;
};

