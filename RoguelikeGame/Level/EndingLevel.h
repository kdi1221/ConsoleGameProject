#pragma once

#include "Level/Level.h"
#include "UI/MainMenu/MainMenu.h"

class MainMenu;

/* 엔딩 레벨 */
class EndingLevel : public Craft::Level
{
	TYPE_DECLARATIONS(EndingLevel, Level)

private:
	static const std::wstring titleAsciiArts[];
	static const std::wstring subTitleAsciiArts[];

public:
	EndingLevel();
	virtual ~EndingLevel();

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

