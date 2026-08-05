#include "Engine/Engine.h"
#include "Level/TestLevel.h"

using namespace Craft;

int main(void)
{
	/*const int FrameStringLength = 256;
	char szFrame[FrameStringLength] = {};
	sprintf_s(szFrame, FrameStringLength, "")

	SetConsoleTitleA("Sokoban Game");*/

	Engine engine;
	engine.AddNewLevel<TestLevel>();
	engine.Run();
}