#include "IngameMenu.h"
#include "UI/BackgroundWidget.h"
#include "Types/Enums.h"
#include <Level/Level.h>
#include <Engine/Engine.h>
#include <Engine/Config/ConfigBase.h>
#include <Render/Renderer.h>
#include <Core/Input.h>
#include <cassert>

using namespace Craft;

IngameMenu::IngameMenu()
{

}

IngameMenu::~IngameMenu()
{
}

void IngameMenu::CreateIngameMenu(std::weak_ptr<Level> activeLevel)
{
	std::shared_ptr<Level> level = activeLevel.lock();
	if (!level)
	{
		return;
	}

	const ConfigBase& configBase = Engine::Get().GetConfig<Craft::ConfigBase>();
	const int viewWidth = configBase.GetViewWidth();
	const int halfWidth =  viewWidth >> 1;

	const int viewHeight = configBase.GetViewHeight();
	const int halfHeight = viewHeight >> 1;

	positionLT.x = halfWidth - (menuWidth >> 1);
	positionLT.y = halfHeight - (menuHeight >> 1);

	/* Background 위젯 */
	std::shared_ptr<BackgroundWidget> ingameBackground = level->CreateWidget<BackgroundWidget>(positionLT, menuWidth, menuHeight, Color::Yellow);
	assert(ingameBackground && "ingameBackground create fail");
	ingameBackground->SetRenderSortingOrder(static_cast<int>(eRenderSortingOrder::UI));
	ingameBackground->SetActive(false);

	background = ingameBackground;
}

void IngameMenu::AddIngameMenuItem(const std::wstring& text, FMenuItem::OnSelected onSelected)
{
	menuList.emplace_back(std::make_unique<FMenuItem>(text, onSelected));
}

void IngameMenu::Tick(float deltaTime)
{
	if (!bShowMenu)
	{
		return;
	}

	const int length = static_cast<int>(menuList.size());
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
		menuList[currentIndex]->onSelected();
	}
}

void IngameMenu::Draw()
{
	if (!bShowMenu)
	{
		return;
	}

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
		Renderer::Get().SubmitUI(menuList[ix]->text, static_cast<Vector2Int>(drawMenuPos), textColor, static_cast<int>(eRenderSortingOrder::UI));
	}
}

void IngameMenu::ShowIngameMenu(bool bShow)
{
	bShowMenu = bShow;
	currentIndex = 0;

	if (std::shared_ptr<BackgroundWidget> ingameBackground = background.lock())
	{
		ingameBackground->SetActive(bShowMenu);
	}

}
