#pragma once

#include "TilemapLevel.h"
#include "UI/IngameMenu/IngameMenu.h"

class IngameMenu;

//실제 인게임 레벨
class GameLevel : public TilemapLevel
{
	TYPE_DECLARATIONS(GameLevel, TilemapLevel)

public:
	GameLevel() = default;
	virtual ~GameLevel() = default;

private:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void OnResumeGame();
	void OnToMenu();
	void OnQuitGame();

private:
	void ShowIngameMenu(bool bShow);

private:
	std::unique_ptr<IngameMenu> ingameMenu;
};

