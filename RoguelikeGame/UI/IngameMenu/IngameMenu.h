#pragma once

#include <Math/Vector2Int.h>
#include <Math/Color.h>
#include <memory>
#include <vector>
#include "UI/MenuItem.h"


namespace Craft
{
	class Level;
}

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

