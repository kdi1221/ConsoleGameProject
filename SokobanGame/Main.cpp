#include "Engine/Engine.h"
#include "Level/GameLevel.h"

using namespace Craft;

int main(void)
{
	Engine engine;
	engine.AddNewLevel<GameLevel>();
	engine.Run();
}