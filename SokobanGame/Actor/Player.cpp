#include "Player.h"
#include "Core/Input.h"
#include "Interface/ICanPlayerMove.h"
#include "Level/Level.h"
#include "Game/Game.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include <iostream>
#include <windows.h>

using namespace Craft;

Player::Player(const Craft::Vector2Float& position)
	:Actor(position)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("P", Color::Green, 5);
	AddComponent<BoxCollisionComponent>(1);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//ESC 키 종료
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game& game = dynamic_cast<Game&>(Engine::Get());
		game.ToggleMenu();
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
		Vector2Float currentPosition = GetPosition();
		Vector2Float newPosition = currentPosition;
		newPosition.x -= 1.f;

		// 새 위치가 이동 가능한지 확인.
		if(canPlayerMove->CanMove(static_cast<Vector2Int>(currentPosition), static_cast<Vector2Int>(newPosition)))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		//이동하려는 새 위치 구하기.
		Vector2Float currentPosition = GetPosition();
		Vector2Float newPosition = currentPosition;
		newPosition.x += 1.f;

		// 새 위치가 이동 가능한지 확인.
		if (canPlayerMove->CanMove(static_cast<Vector2Int>(currentPosition), static_cast<Vector2Int>(newPosition)))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		//이동하려는 새 위치 구하기.
		Vector2Float currentPosition = GetPosition();
		Vector2Float newPosition = currentPosition;
		newPosition.y -= 1.f;

		// 새 위치가 이동 가능한지 확인.
		if (canPlayerMove->CanMove(static_cast<Vector2Int>(currentPosition), static_cast<Vector2Int>(newPosition)))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		//이동하려는 새 위치 구하기.
		Vector2Float currentPosition = GetPosition();
		Vector2Float newPosition = currentPosition;
		newPosition.y += 1.f;

		// 새 위치가 이동 가능한지 확인.
		if (canPlayerMove->CanMove(static_cast<Vector2Int>(currentPosition), static_cast<Vector2Int>(newPosition)))
		{
			//이동 가능한 경우에는 이동 처리
			SetPosition(newPosition);
		}
	}


	//std::cout << "Player::Tick() - deltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';
}
