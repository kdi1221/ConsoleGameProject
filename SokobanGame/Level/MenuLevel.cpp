#include "MenuLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Render/Renderer.h"

using namespace Craft;

MenuLevel::MenuLevel()
{
	// 메뉴 아이템 생성 및 배열에 추가.
	itemList.emplace_back(
		std::make_unique<MenuItem>(
			L"Resume Game",
			[]()
			{
				Game& game = dynamic_cast<Game&>(Engine::Get());
				game.ToggleMenu();
			}
		)
	);

	itemList.emplace_back(
		std::make_unique<MenuItem>(
			L"Quit Game",
			[]()
			{
				Engine::Get().Quit();
			}
		)
	);
}

void MenuLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	// 입력 처리(위/아래 방향키, 엔터키, ESC키)
	const int length = static_cast<int>(itemList.size());
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 감소처리, 인덱스 돌리기.
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// 인덱스 증가처리, 인덱스 돌리기.
		currentIndex = (currentIndex + 1) % length;
	}

	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장한 함수(함수 포인터로 저장) 호출.
		itemList[currentIndex]->onSelected();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글.
		Game& game = dynamic_cast<Game&>(Engine::Get());
		game.ToggleMenu();

		//인덱스 초기화.
		currentIndex = 0;
	}
}

void MenuLevel::Draw()
{
	Level::Draw();

	// 메뉴 제목.
	Renderer::Get().Submit(L"Sokoban Game", Vector2Float::Zero);

	// 메뉴 아이템 출력
	const int count = static_cast<int>(itemList.size());
	for (int ix = 0; ix < count; ++ix)
	{
		// 아이템 색상 확인.
		Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor;

		// 메뉴 목록 그리기
		const Vector2Int drawMenuPos(0, 2 + ix);
		Renderer::Get().Submit(itemList[ix]->text, static_cast<Vector2Float>(drawMenuPos), textColor);
	}
}
