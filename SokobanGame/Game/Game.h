#pragma once

#include "Engine/Engine.h"
#include <vector>


// 레벨 관리에 사용할 열거형.
enum class State
{
	GamePlay = 0,
	Menu = 1,
	Length
};

class Game : public Craft::Engine
{
public:
	Game();
	~Game() = default;

public:
	// 메뉴/게임 레벨을 전환하는 함수.
	void ToggleMenu();

private:
	//게임에서 관리하는 레벨을 저장할 배열.
	std::vector<std::shared_ptr<Craft::Level>> levelList;

	//현재 활성화된 레벨을 나타내는 열거형.
	State state = State::GamePlay;
};

