#include "MainMenuLevel.h"
#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "Render/Renderer.h"
#include <Engine/Engine.h>
#include <cassert>

using namespace Craft;

static const std::wstring titleAsciiArts[] =
{
 L" _____    ____    _____  _    _  ______    __          __ _____  ______     ___      _____   _____",
 L"|  __ ⧵  / __ ⧵  / ____|| |  | ||  ____|   ⧵ ⧵        / /|_   _||___  /    /   ⧵    |  __ ⧵ |  __ ⧵ ",
 L"| |__) || |  | || |  __ | |  | || |__       ⧵ ⧵  /⧵  / /   | |     / /    /  ^  ⧵   | |__) || |  | |",
 L"|  _  / | |  | || | |_ || |  | ||  __|       ⧵ ⧵/  ⧵/ /    | |    / /    /  /_⧵  ⧵  |  _  / | |  | |",
 L"| | ⧵ ⧵ | |__| || |__| || |__| || |____       ⧵  /⧵  /    _| |_  / /__  /  _____  ⧵ | | ⧵ ⧵ | |__| |",
 L"|_|  ⧵_⧵ ⧵____/  ⧵_____| ⧵____/ |______|       ⧵/  ⧵/    |_____|/_____|/__/     ⧵__⧵|_|  ⧵_⧵|_____/ ",
 L"                                                                       |__|     |__|                "
};

MainMenuLevel::MainMenuLevel()
{
}

MainMenuLevel::~MainMenuLevel()
{
}

void MainMenuLevel::OnInitialized()
{
	super::OnInitialized();

	mainMenu = std::make_unique<MainMenu>();
	assert(mainMenu && "Invalid mainMenu");
	mainMenu->CreateMenu(weak_from_this());
	mainMenu->AddMenuItem(L"Play", std::bind(&MainMenuLevel::OnPlay, this));
	mainMenu->AddMenuItem(L"Quit Game", std::bind(&MainMenuLevel::OnQuitGame, this));

	Engine::Get().PlayBackgroundMusic("BGM/Mainmenu.wav");
}

void MainMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (mainMenu)
	{
		mainMenu->Tick(deltaTime);
	}
}

void MainMenuLevel::Draw()
{
	super::Draw();

	Vector2Int drawPos(25, 5);
	for(const std::wstring& titleAscii : titleAsciiArts)
	{
		Renderer::Get().SubmitUI(titleAscii, drawPos, Color::BrightYellow);
		++drawPos.y;
	}

	if (mainMenu)
	{
		mainMenu->Draw();
	}
}

void MainMenuLevel::OnPlay()
{
	//인게임 레벨 전환
	Game& game = dynamic_cast<Game&>(Engine::Get());

	game.InitializeGameSessionData();
	game.AddNewLevel<GameLevel>();
}

void MainMenuLevel::OnQuitGame()
{
	Engine::Get().Quit();
}