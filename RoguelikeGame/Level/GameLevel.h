#pragma once

#include "TilemapLevel.h"
#include "UI/IngameMenu/IngameMenu.h"
#include "UI/PlayerDeathMenu/PlayerDeathMenu.h"

class PS_Roguelike;
class IngameMenu;
class PlayerDeathMenu;


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

public:
	void OnPlayerDeath(const PS_Roguelike& playerState);

private:
	void OnResumeGame();
	void OnToMenu();
	void OnQuitGame();

private:
	void ShowIngameMenu(bool bShow);
	void ShowPlayerDeathMenu(const PS_Roguelike& playerState);

private:
	std::unique_ptr<IngameMenu> ingameMenu;
	std::unique_ptr<PlayerDeathMenu> playerDeathMenu;
};

