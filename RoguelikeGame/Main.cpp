#include "Game/Game.h"
#include "Level/GameLevel.h"
#include "Level/MainMenuLevel.h"

using namespace Craft;

int main(void)
{
	SetConsoleTitle(L"Rogue Wizard");


	// 콘솔 출력 인코딩을 UTF-8로 설정
	//SetConsoleOutputCP(65001);
	//SetConsoleCP(65001);
	
	Game game;
	game.InitializeEngine();
	
	//game.AddNewLevel<MainMenuLevel>();

	//테스트 : 인게임 곧바로 시작
	game.InitializeGameSessionData();
	game.AddNewLevel<GameLevel>();

	game.Run();
}