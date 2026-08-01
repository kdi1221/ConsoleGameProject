#include "Player.h"
#include "Core/Input.h"
#include "Interface/ICanPlayerMove.h"
#include "Level/Level.h"
#include <iostream>
#include <windows.h>

using namespace Craft;

Player::Player(const Craft::Vector2& position)
	:Actor("P", position, Color::Green)
{
	sortingOrder = 5;
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//ESC 키 종료
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	std::shared_ptr<ICanPlayerMove> canPlayerMove = std::dynamic_pointer_cast<ICanPlayerMove>(GetOwner());
	if (!canPlayerMove)
	{
		return;
	}

	//방향키 이동.
	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		//이동하려는 새 위치 구하기.
		Vector2 newPosition = position;
		newPosition.x -= 1;

		// 새 위치가 이동 가능한지 확인.
		if(canPlayerMove->CanMove(position, newPosition))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		//이동하려는 새 위치 구하기.
		Vector2 newPosition = position;
		newPosition.x += 1;

		// 새 위치가 이동 가능한지 확인.
		if (canPlayerMove->CanMove(position, newPosition))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		//이동하려는 새 위치 구하기.
		Vector2 newPosition = position;
		newPosition.y -= 1;

		// 새 위치가 이동 가능한지 확인.
		if (canPlayerMove->CanMove(position, newPosition))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		//이동하려는 새 위치 구하기.
		Vector2 newPosition = position;
		newPosition.y += 1;

		// 새 위치가 이동 가능한지 확인.
		if (canPlayerMove->CanMove(position, newPosition))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}


	//std::cout << "Player::Tick() - deltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';
}
