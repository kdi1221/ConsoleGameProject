#include "GameLevel.h"
#include "Game/Game.h"
#include "Level/MainMenuLevel.h"
#include <Core/Input.h>
#include <cassert>

using namespace Craft;

void GameLevel::OnInitialized()
{
	super::OnInitialized();

	ingameMenu = std::make_unique<IngameMenu>();
	assert(ingameMenu && "Invalid ingameMenu");
	ingameMenu->CreateIngameMenu(weak_from_this());
	ingameMenu->AddIngameMenuItem(L"Resume Game", std::bind(&GameLevel::OnResumeGame, this));
	ingameMenu->AddIngameMenuItem(L"To Menu", std::bind(&GameLevel::OnToMenu, this));
	ingameMenu->AddIngameMenuItem(L"Quit Game",std::bind(&GameLevel::OnQuitGame, this));

	playerDeathMenu = std::make_unique<PlayerDeathMenu>();
	assert(playerDeathMenu && "Invalid playerDeathMenu");
	playerDeathMenu->CreatePlayerDeathMenu(weak_from_this());
	playerDeathMenu->AddPlayerDeathMenuItem(L"To Menu", std::bind(&GameLevel::OnToMenu, this));
	playerDeathMenu->AddPlayerDeathMenuItem(L"Quit Game", std::bind(&GameLevel::OnQuitGame, this));
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//메인메뉴 키 입력..
	const Input& input = Input::Get();
	if (input.GetKeyDown(VK_ESCAPE))
	{
		SetGamePause(!GetGamePaused());
		ShowIngameMenu(GetGamePaused());
	}

	if (ingameMenu)
	{
		ingameMenu->Tick(deltaTime);
	}

	if (playerDeathMenu)
	{
		playerDeathMenu->Tick(deltaTime);
	}
}

void GameLevel::Draw()
{
	super::Draw();

	if (ingameMenu)
	{
		ingameMenu->Draw();
	}

	if (playerDeathMenu)
	{
		playerDeathMenu->Draw();
	}
}

void GameLevel::OnPlayerDeath(const PS_Roguelike& playerState)
{
	SetGamePause(true);

	ShowPlayerDeathMenu(playerState);
}

void GameLevel::OnResumeGame()
{
	SetGamePause(false);
	ShowIngameMenu(GetGamePaused());
}

void GameLevel::OnToMenu()
{
	Game& game = dynamic_cast<Game&>(Engine::Get());
	game.DestroyGameSessionData();
	game.AddNewLevel<MainMenuLevel>();
}

void GameLevel::OnQuitGame()
{
	Engine::Get().Quit();
}

void GameLevel::ShowIngameMenu(bool bShow)
{
	if (!ingameMenu)
	{
		return;
	}

	ingameMenu->ShowIngameMenu(bShow);
}

void GameLevel::ShowPlayerDeathMenu(const PS_Roguelike& playerState)
{
	if (!playerDeathMenu)
	{
		return;
	}

	playerDeathMenu->ShowPlayerDeathMenu(true);
}
