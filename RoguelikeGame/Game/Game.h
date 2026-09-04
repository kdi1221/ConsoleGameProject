#pragma once

#include "Engine/Engine.h"
#include <memory>
#include <string>

class ConfigBase;
class Game : public Craft::Engine
{
public:
	Game();
	~Game() = default;

private:
	// 충돌 처리 함수
	virtual void ProcessColiision() override;

	// 설정 생성 반환 함수(별도의 커스텀 설정 반환)
	virtual std::unique_ptr<Craft::ConfigBase> CreateConfig() const override;

	// 리소스 매니저 생성 반환 함수
	virtual std::unique_ptr<Craft::ResourceManager> CreateResourceManager() const override;

	/* 네비게이션 시스템 객체 생성 */
	virtual std::unique_ptr<Craft::NavigationBase> CreateNavigationSystem() const override;

	/* 커스텀 게임 모드 객체 생성  */
	virtual std::unique_ptr<Craft::GameMode> CreateGameMode() const override;
};

