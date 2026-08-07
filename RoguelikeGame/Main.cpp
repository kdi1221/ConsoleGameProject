#include "Game/Game.h"
#include "Level/TestLevel.h"

using namespace Craft;

int main(void)
{
	/*const int FrameStringLength = 256;
	char szFrame[FrameStringLength] = {};
	sprintf_s(szFrame, FrameStringLength, "")

	SetConsoleTitleA("Sokoban Game");*/

	Game game;
	game.InitializeEngine();
	game.AddNewLevel<TestLevel>();
	game.Run();
}