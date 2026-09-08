#pragma once

#include "Level/Level.h"

class MainMenu;

/* 게임오버 레벨 */
class GameOverLevel : public Craft::Level
{
	TYPE_DECLARATIONS(GameOverLevel, Level)

private:
	static const std::wstring titleAsciiArts[];

public:
	GameOverLevel();
	virtual ~GameOverLevel();

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void OnReturnTitle();
	void OnExitGame();

private:
	std::unique_ptr<MainMenu> mainMenu;
};