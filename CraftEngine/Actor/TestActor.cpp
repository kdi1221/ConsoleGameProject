#include "TestActor.h"
#include <iostream>

void TestActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	std::cout << "TestActor::Tick() - deltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';
}
