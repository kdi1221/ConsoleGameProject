#include "MainMenu.h"
#include "UI/BackgroundWidget.h"
#include "Types/Enums.h"
#include <Level/Level.h>
#include <Engine/Engine.h>
#include <Engine/Config/ConfigBase.h>
#include <Render/Renderer.h>
#include <Core/Input.h>
#include <cassert>

using namespace Craft;

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{

}

void MainMenu::CreateMenu(std::weak_ptr<Craft::Level> activeLevel)
{
	std::shared_ptr<Level> level = activeLevel.lock();
	if (!level)
	{
		return;
	}

	const ConfigBase& configBase = Engine::Get().GetConfig<Craft::ConfigBase>();
	const int viewWidth = configBase.GetDisplayWidth();
	const int halfWidth = viewWidth >> 1;

	const int viewHeight = configBase.GetDisplayHeight();

	positionLT.x = halfWidth - (menuWidth >> 1);
	positionLT.y = viewHeight - menuHeight - 7;

	/* Background 위젯 */
	std::shared_ptr<BackgroundWidget> ingameBackground = level->CreateWidget<BackgroundWidget>(positionLT, menuWidth, menuHeight, Color::Yellow);
	assert(ingameBackground && "ingameBackground create fail");
	ingameBackground->SetRenderSortingOrder(static_cast<int>(eRenderSortingOrder::UI));
	ingameBackground->SetActive(true);

	background = ingameBackground;
}

void MainMenu::AddMenuItem(const std::wstring& text, FMenuItem::OnSelected onSelected)
{
	menuList.emplace_back(std::make_unique<FMenuItem>(text, onSelected));
}

void MainMenu::Tick(float deltaTime)
{
	const int length = static_cast<int>(menuList.size());
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 감소처리, 인덱스 돌리기.
		currentIndex = (currentIndex - 1 + length) % length;

		Engine::Get().PlayOneShot("Effect/selectmove.wav");
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// 인덱스 증가처리, 인덱스 돌리기.
		currentIndex = (currentIndex + 1) % length;

		Engine::Get().PlayOneShot("Effect/selectmove.wav");
	}

	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장한 함수(함수 포인터로 저장) 호출.
		menuList[currentIndex]->onSelected();

		Engine::Get().PlayOneShot("Effect/button.wav");
	}
}

void MainMenu::Draw()
{
	const int menuHalfWidth = menuWidth >> 1;

	// 메뉴 아이템 출력
	const int count = static_cast<int>(menuList.size());
	for (int ix = 0; ix < count; ++ix)
	{
		// 아이템 색상 확인.
		Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor;

		// 메뉴 목록 그리기
		int drawXPos = positionLT.x + menuHalfWidth - (static_cast<int>(menuList[ix]->text.length()) >> 1);

		const Vector2Int drawMenuPos(drawXPos, positionLT.y + 2 + (ix * 3));
		Renderer::Get().SubmitUI(menuList[ix]->text, drawMenuPos, textColor, static_cast<int>(eRenderSortingOrder::UI));
	}
}
