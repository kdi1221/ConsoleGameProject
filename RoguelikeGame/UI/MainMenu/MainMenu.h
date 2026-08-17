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

class MainMenu
{
public:
	MainMenu();
	virtual ~MainMenu();

public:
	void CreateMenu(std::weak_ptr<Craft::Level> activeLevel);
	void AddMenuItem(const std::wstring& text, FMenuItem::OnSelected onSelected);

	void Tick(float deltaTime);
	void Draw();

private:
	std::weak_ptr<BackgroundWidget> background;
	std::vector<std::unique_ptr<FMenuItem>> menuList;

	Craft::Vector2Int positionLT = Craft::Vector2Int::Zero;
	int menuWidth = 30;
	int menuHeight = 7;

	int currentIndex = 0;
	Craft::Color selectedColor = Craft::Color::Yellow;
	Craft::Color unselectedColor = Craft::Color::White;
};

