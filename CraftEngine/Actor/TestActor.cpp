#include "TestActor.h"
#include "Core/Input.h"
#include <iostream>
#include <windows.h>

using namespace Craft;

void TestActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	//ESC 키 종료
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	if (Input::Get().GetKeyDown('A'))
	{
		std::cout << "A key is down\n";
	}

	if (Input::Get().GetKey('A'))
	{
		std::cout << "A key is holding down\n";
	}

	if (Input::Get().GetKeyUp('A'))
	{
		std::cout << "A key is Up\n";
	}

	//std::cout << "TestActor::Tick() - deltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';
}
