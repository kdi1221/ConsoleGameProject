#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "Level/MainMenuLevel.h"
#include "Level/DummyLevel.h"
#include <Resource/ResourceManager.h>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

using namespace Craft;

int main(void)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(355);

	SetConsoleTitle(L"Rogue Wizard");


	// 콘솔 출력 인코딩을 UTF-8로 설정
	//SetConsoleOutputCP(65001);
	//SetConsoleCP(65001);
	
	Game game;
	game.InitializeEngine();

	//리소스 로드
	ResourceManager& resourceManager = game.GetResourceManager<ResourceManager>();
	if (!resourceManager.LoadResources())
	{
		return -1;
	}
	
	//game.AddNewLevel<MainMenuLevel>();

	//테스트 : 인게임 곧바로 시작
	game.InitializeGameSessionData();
	//game.AddNewLevel<GameLevel>(TilemapLevel::eLevelCategory::MAZE_BSP);
	game.AddNewLevel<GameLevel>(TilemapLevel::eLevelCategory::BOSS_ROOM);

	//더미 - 프레임 테스트용
	//game.AddNewLevel<DummyLevel>();

	game.Run();
}