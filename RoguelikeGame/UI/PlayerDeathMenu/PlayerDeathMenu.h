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

class PlayerDeathMenu
{
public:
	PlayerDeathMenu();
	virtual ~PlayerDeathMenu();

public:
	void CreatePlayerDeathMenu(std::weak_ptr<Craft::Level> activeLevel);
	void AddPlayerDeathMenuItem(const std::wstring& text, FMenuItem::OnSelected onSelected);
	void ShowPlayerDeathMenu(bool bShow);

	void Tick(float deltaTime);
	void Draw();

private:
	bool bShowMenu = false;
	std::weak_ptr<BackgroundWidget> background;
	std::vector<std::unique_ptr<FMenuItem>> menuList;

	Craft::Vector2Int positionLT = Craft::Vector2Int::Zero;
	int menuWidth = 30;
	int menuHeight = 15;

	int currentIndex = 0;
	Craft::Color selectedColor = Craft::Color::Yellow;
	Craft::Color unselectedColor = Craft::Color::White;

	std::wstring gameEndText = L"YOU DIED";
};

