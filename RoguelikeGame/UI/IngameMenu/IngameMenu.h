#pragma once

#include <Math/Vector2Int.h>
#include <Math/Color.h>
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace Craft
{
	class Level;
}

//메뉴 아이템 구조체.
struct FMenuItem
{
	// 메뉴가 선택됐을 때 실행될 함수의 타입(함수 포인터).
	using OnSelected = std::function<void()>;

	FMenuItem(const std::wstring& text, OnSelected onSelected)
		:text(text), onSelected(onSelected)
	{

	}

	~FMenuItem() = default;

	// 속성(메뉴 텍스트)
	std::wstring text;

	//메뉴가 선택됐을 때 실행될 로직(함수).
	OnSelected onSelected = nullptr;
};

class BackgroundWidget;

class IngameMenu
{
public:
	IngameMenu();
	virtual ~IngameMenu();

public:
	void CreateIngameMenu(std::weak_ptr<Craft::Level> activeLevel);
	void AddIngameMenuItem(const std::wstring& text, FMenuItem::OnSelected onSelected);
	void ShowIngameMenu(bool bShow);

	void Tick(float deltaTime);
	void Draw();

private:
	bool bShowMenu = false;
	std::weak_ptr<BackgroundWidget> background;
	std::vector<std::unique_ptr<FMenuItem>> menuList;

	Craft::Vector2Int positionLT = Craft::Vector2Int::Zero;
	int menuWidth = 30;
	int menuHeight = 10;

	int currentIndex = 0;
	Craft::Color selectedColor = Craft::Color::Yellow;
	Craft::Color unselectedColor = Craft::Color::White;
};

