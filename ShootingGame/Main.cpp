#include "Engine/Engine.h"
#include "Level/GameLevel.h"
#include "Actor/Player.h"

#include <iostream>

using namespace Craft;

int main(void)
{
	Engine engine;
	engine.AddNewLevel<GameLevel>();
	engine.Run();

	//사피네
	/*auto isPlayerChild = std::is_base_of<Actor, Player>::value;
	auto result = std::is_base_of<Actor, Level>::value;

	std::enable_if<true, Player>::type;

	std::cin.get();*/
}