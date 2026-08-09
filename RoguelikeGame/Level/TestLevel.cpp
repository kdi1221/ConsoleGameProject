#include "TestLevel.h"
#include "Engine/Engine.h"
#include "Core/Input.h"

void TestLevel::Tick(float deltaTime)
{
	TilemapLevel::Tick(deltaTime);

	//다음 레벨 설정
	/*if (Craft::Input::Get().GetKeyDown('B'))
	{
		Craft::Engine::Get().AddNewLevel<TestLevel>();
	}*/
}
