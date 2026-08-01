#include "Player.h"
#include "Engine/Engine.h"
#include "Core/Input.h"

using namespace Craft;

Player::Player()
	:Actor("<=A=>", Vector2::Zero, Color::Green)
{
	// 생성 위치 설정.
	int x = (Engine::Get().GetWidth() / 2) - (width / 2);
	int y = Engine::Get().GetHeight() - 2;
	SetPosition(Vector2(x, y));

	// x위치 저장.
	xPosition = static_cast<float>(x);

}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 종료 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	// 플레이어가 이동할 방향 설정.
	// 오른쪽 방향키 -> 이동 방향 1
	// 왼쪽 방향키 -> 이동 방향 -1
	float direction = 0.f;
	if (Input::Get().GetKey(VK_RIGHT))
	{
		direction = 1.f;
	}

	if (Input::Get().GetKey(VK_LEFT))
	{
		direction = -1.f;
	}

	//이동 처리.
	Move(direction, deltaTime);
}

void Player::Move(float direction, float deltaTime)
{
	//x 위치 업데이트.
	xPosition = xPosition + direction * moveSpeed * deltaTime;

	// 화면 왼쪽 벗어나지 않도록 고정.
	if (xPosition < 0.f)
	{
		xPosition = 0.f;
	}

	// 화면 오른쪽 벗어나지 않도록 고정.
	if (xPosition + width >= Engine::Get().GetWidth())
	{
		xPosition = static_cast<float>(Engine::Get().GetWidth() - width);
	}

	// 위치 업데이트.
	Vector2 newPosition = position;
	newPosition.x = static_cast<int>(xPosition);
	SetPosition(newPosition);
}