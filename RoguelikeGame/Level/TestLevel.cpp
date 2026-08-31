#include "TestLevel.h"
#include "Engine/Engine.h"
#include "Core/Input.h"

TestLevel::~TestLevel()
{
	
}

void TestLevel::OnInitialized()
{
	/*Craft::Engine::Get().DestroyGameSessionData();
	Craft::Engine::Get().InitializeGameSessionData();*/

	super::OnInitialized();
}

void TestLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//다음 레벨 설정
	/*if (Craft::Input::Get().GetKeyDown('B'))
	{
		Craft::Engine::Get().AddNewLevel<TestLevel>();
	}*/
}
