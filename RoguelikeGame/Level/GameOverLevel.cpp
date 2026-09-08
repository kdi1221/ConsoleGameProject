#include "GameOverLevel.h"

#include "Game/Game.h"
#include "Level/MainMenuLevel.h"
#include <Render/Renderer.h>
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

const std::wstring GameOverLevel::titleAsciiArts[] =
{
L"██      ██    ██████    ██      ██        ████████    ██████  ██████████  ████████",
L"██      ██    ██████    ██      ██        ████████    ██████  ██████████  ████████",
L"  ██  ██    ██      ██  ██      ██        ██      ██    ██    ██          ██      ██",
L"  ██  ██    ██      ██  ██      ██        ██      ██    ██    ██          ██      ██",
L"    ██      ██      ██  ██      ██        ██      ██    ██    ████████    ██      ██",
L"    ██      ██      ██  ██      ██        ██      ██    ██    ████████    ██      ██",
L"    ██      ██      ██  ██      ██        ██      ██    ██    ██          ██      ██",
L"    ██      ██      ██  ██      ██        ██      ██    ██    ██          ██      ██",
L"    ██        ██████      ██████          ████████    ██████  ██████████  ████████",
L"    ██        ██████      ██████          ████████    ██████  ██████████  ████████"
};

GameOverLevel::GameOverLevel()
{

}

GameOverLevel::~GameOverLevel()
{

}

void GameOverLevel::OnInitialized()
{
	super::OnInitialized();

	mainMenu = std::make_unique<MainMenu>();
	assert(mainMenu && "Invalid mainMenu");
	mainMenu->CreateMenu(weak_from_this());
	mainMenu->AddMenuItem(L"RETURN TO TITLE ", std::bind(&GameOverLevel::OnReturnTitle, this));
	mainMenu->AddMenuItem(L"EXIT GAME", std::bind(&GameOverLevel::OnExitGame, this));

	Engine::Get().PlayOneShot("BGM/GameOver.wav");
}

void GameOverLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (mainMenu)
	{
		mainMenu->Tick(deltaTime);
	}
}

void GameOverLevel::Draw()
{
	super::Draw();

	Vector2Int titleDrawPos(48, 5);
	for (const std::wstring& titleAscii : titleAsciiArts)
	{
		Renderer::Get().SubmitUI(titleAscii, titleDrawPos, Color::Red);
		++titleDrawPos.y;
	}

	if (mainMenu)
	{
		mainMenu->Draw();
	}
}

void GameOverLevel::OnReturnTitle()
{
	Game& game = dynamic_cast<Game&>(Engine::Get());
	game.DestroyGameSessionData();
	game.AddNewLevel<MainMenuLevel>();
}

void GameOverLevel::OnExitGame()
{
	Engine::Get().Quit();
}