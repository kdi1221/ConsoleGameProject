#include "Game/Game.h"
#include <Windows.h>

using namespace Craft;

int main(void)
{
	//창 제목 설정
	SetConsoleTitleA("Sokoban Game");

	Game game;
	game.Run();
}