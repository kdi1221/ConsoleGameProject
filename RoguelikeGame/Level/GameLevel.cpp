#include "GameLevel.h"
#include <Core/Input.h>
#include <cassert>
#include <Engine/Engine.h>

using namespace Craft;

void GameLevel::OnInitialized()
{
	super::OnInitialized();

	ingameMenu = std::make_unique<IngameMenu>();
	assert(ingameMenu && "Invalid ingameMenu");

	ingameMenu->CreateIngameMenu(weak_from_this());

	ingameMenu->AddIngameMenuItem(L"Resume Game", std::bind(&GameLevel::OnResumeGame, this));
	ingameMenu->AddIngameMenuItem(L"To Menu", std::bind(&GameLevel::OnToMenu, this));
	ingameMenu->AddIngameMenuItem(L"Quit Game", std::bind(&GameLevel::OnQuitGame, this));
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
}

void GameLevel::Draw()
{
	super::Draw();

	if (ingameMenu)
	{
		ingameMenu->Draw();
	}
}

void GameLevel::OnResumeGame()
{
	SetGamePause(false);
	ShowIngameMenu(GetGamePaused());
}

void GameLevel::OnToMenu()
{
	OutputDebugStringA("TODO : To Menu");
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