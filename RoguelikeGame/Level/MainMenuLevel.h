#pragma once

#include "Level/Level.h"
#include "UI/MainMenu/MainMenu.h"


/* 메인메뉴 레벨*/
class MainMenuLevel : public Craft::Level
{
	TYPE_DECLARATIONS(MainMenuLevel, Level)

public:
	MainMenuLevel();
	virtual ~MainMenuLevel();

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void OnPlay();
	void OnQuitGame();

private:
	std::unique_ptr<MainMenu> mainMenu;
};

