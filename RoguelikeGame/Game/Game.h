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
	// 게임 플레이 업데이트 함수.
	virtual void Tick(float deltaTime);

	// 화면에 그리는 함수.
	virtual void Draw();

	// 충돌 처리 함수
	virtual void ProcessColiision() override;

	// 설정 생성 반환 함수(별도의 커스텀 설정 반환)
	virtual std::unique_ptr<Craft::ConfigBase> CreateConfig() const override;

	/* 커스텀 게임 모드 객체 생성  */
	virtual std::unique_ptr<Craft::GameMode> CreateGameMode() const override;

	/* 커스텀 플레이어 상태 객체 생성 */
	virtual std::unique_ptr<Craft::PlayerState> CreatePlayerState() const override;

private:
	void FormatCurrentFPSString(const float deltaTime);
	void DrawFPSString();

private:
	/* 출력할 FPS 문자열 */
	std::wstring szFPS;
};

