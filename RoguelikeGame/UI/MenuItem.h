#pragma once

#include <functional>
#include <string>

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