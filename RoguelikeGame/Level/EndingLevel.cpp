#include "EndingLevel.h"
#include "Game/Game.h"
#include "Level/MainMenuLevel.h"
#include <Render/Renderer.h>
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

const std::wstring EndingLevel::titleAsciiArts[] =
{
	L" ███   ███  █   █  ███  ████   ███  █████ █   █ █      ███  █████ ███  ███  █   █  ████ █",
	L"█     █   █ ██  █ █     █   █ █   █   █   █   █ █     █   █   █    █  █   █ ██  █ █     █",
	L"█     █   █ █ █ █ █  ██ ████  █████   █   █   █ █     █████   █    █  █   █ █ █ █  ███  █",
	L"█     █   █ █  ██ █   █ █  █  █   █   █   █   █ █     █   █   █    █  █   █ █  ██     █",
	L" ███   ███  █   █  ███  █   █ █   █   █    ███  █████ █   █   █   ███  ███  █   █ ████  █"
};

const std::wstring EndingLevel::subTitleAsciiArts[] =
{
	L"█   █  ███  █   █    █████  ████  ███   ███  ████  █████ ████     █████ █   █ █████    ████  █   █ █   █  ███  █████  ███  █   █ █",
	L" █ █  █   █ █   █    █     █     █     █   █ █   █ █     █   █      █   █   █ █        █   █ █   █ ██  █ █     █     █   █ ██  █ █",
	L"  █   █   █ █   █    ████   ███  █     █████ ████  ████  █   █      █   █████ ████     █   █ █   █ █ █ █ █  ██ ████  █   █ █ █ █ █",
	L"  █   █   █ █   █    █         █ █     █   █ █     █     █   █      █   █   █ █        █   █ █   █ █  ██ █   █ █     █   █ █  ██",
	L"  █    ███   ███     █████ ████   ███  █   █ █     █████ ████       █   █   █ █████    ████   ███  █   █  ███  █████  ███  █   █ █"
};



EndingLevel::EndingLevel()
{

}

EndingLevel::~EndingLevel()
{

}

void EndingLevel::OnInitialized()
{
	super::OnInitialized();

	mainMenu = std::make_unique<MainMenu>();
	assert(mainMenu && "Invalid mainMenu");
	mainMenu->CreateMenu(weak_from_this());
	mainMenu->AddMenuItem(L"RETURN TO TITLE ", std::bind(&EndingLevel::OnReturnTitle, this));
	mainMenu->AddMenuItem(L"EXIT GAME", std::bind(&EndingLevel::OnExitGame, this));

	Engine::Get().PlayBackgroundMusic("BGM/Ending.wav");
}

void EndingLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (mainMenu)
	{
		mainMenu->Tick(deltaTime);
	}
}

void EndingLevel::Draw()
{
	super::Draw();

	Vector2Int titleDrawPos(42, 5);
	for (const std::wstring& titleAscii : titleAsciiArts)
	{
		Renderer::Get().SubmitUI(titleAscii, titleDrawPos, Color::LightGreen);
		++titleDrawPos.y;
	}

	Vector2Int subTitleDrawPos(25, 15);
	for (const std::wstring& titleAscii : subTitleAsciiArts)
	{
		Renderer::Get().SubmitUI(titleAscii, subTitleDrawPos, Color::BrightYellow);
		++subTitleDrawPos.y;
	}

	if (mainMenu)
	{
		mainMenu->Draw();
	}
}

void EndingLevel::OnReturnTitle()
{
	Game& game = dynamic_cast<Game&>(Engine::Get());
	game.DestroyGameSessionData();
	game.AddNewLevel<MainMenuLevel>();
}

void EndingLevel::OnExitGame()
{
	Engine::Get().Quit();
}
