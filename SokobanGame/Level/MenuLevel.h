#pragma once

#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"

//메뉴 아이템 구조체.
struct MenuItem
{
	// 메뉴가 선택됐을 때 실행될 함수의 타입(함수 포인터).
	using OnSelected = void(*)();

	MenuItem(const std::wstring& text, OnSelected onSelected)
		:text(text), onSelected(onSelected)
	{

	}

	~MenuItem() = default;

	// 속성(메뉴 텍스트)
	std::wstring text;

	//메뉴가 선택됐을 때 실행될 로직(함수).
	OnSelected onSelected = nullptr;
};

//메뉴를 제공하는 레벨.
class MenuLevel : public Craft::Level
{
public:
	MenuLevel();
	~MenuLevel() = default;

private:
	//이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;	
	virtual void Draw() override;

private:
	// 현재 활성화된 메뉴 목록의 인덱스.
	int currentIndex = 0;

	// 선택된 목록의 색상.
	Craft::Color selectedColor = Craft::Color::Green;

	// 선택되지 않은 목록의 색상
	Craft::Color unselectedColor = Craft::Color::White;

	// 메뉴 아이템 배열.
	std::vector<std::unique_ptr<MenuItem>> itemList;
};

