#pragma once

#include "TilemapLevel.h"

//타일맵 테스트 및 개발용 레벨
class TestLevel : public TilemapLevel
{
public:
	TestLevel() = default;
	virtual ~TestLevel() = default;

private:
	virtual void Tick(float deltaTime) override;
};

