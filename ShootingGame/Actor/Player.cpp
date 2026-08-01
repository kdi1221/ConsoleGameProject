#include "Player.h"
#include "Engine/Engine.h"
#include "Core/Input.h"
#include "PlayerBullet.h"
#include "Level/Level.h"
#include "EnemyBullet.h"
#include "DestroyEffect.h"

using namespace Craft;

Player::Player()
	:Actor("<=A=>", Vector2::Zero, Color::Green)
	,fireMode(FireMode::OneShot)
{
	// 생성 위치 설정.
	int x = (Engine::Get().GetWidth() / 2) - (width / 2);
	int y = Engine::Get().GetHeight() - 2;
	SetPosition(Vector2(x, y));

	// x위치 저장.
	xPosition = static_cast<float>(x);

	// 연사 타이머의 목표 시간 설정.
	fireTimer.SetTargetTime(fireInterval);
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

	//발사 타이머 업데이트
	fireTimer.Tick(deltaTime);

	//탄약 발사 처리.
	if (fireMode == FireMode::OneShot)
	{
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			Fire();
		}
	}
	else if (fireMode == FireMode::Repeat)
	{
		if (Input::Get().GetKey(VK_SPACE))
		{
			FireInterval();
		}
	}

	// 발사 모드 전환 처리
	if (Input::Get().GetKeyDown('R'))
	{
		int mode = static_cast<int>(fireMode);
		mode = 1 - mode;
		fireMode = static_cast<FireMode>(mode);
	}
}

void Player::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	// 충돌한 액터가 적 탄약인지 확인
	if (other->IsTypeOf<EnemyBullet>())
	{
		// 플레이어 제거
		Destroy();

		// 적 탄약 제거
		other->Destroy();

		// 플레이어 죽음 이펙트 재생
		if (GetOwner())
		{
			//플레이어가 죽은 위치에 죽음 이펙트 생성.
			GetOwner()->SpawnActor<DestroyEffect>(position);
		}

		//TODO : 게임 관리자에 플레이어 죽음 알림.
	}
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

void Player::Fire()
{
	//발사할 탄약의 위치 설정(플레이어의 가운데 위치).
	Vector2 bulletPosition(position.x + (width / 2), position.y - 1);

	//탄약 생성 요청.
	GetOwner()->SpawnActor<PlayerBullet>(bulletPosition);
}

void Player::FireInterval()
{
	// 발사 가능 여부 확인
	if (!CanShoot())
	{
		return;
	}

	//타이머 경과 시간 초기화.
	fireTimer.Reset();

	//발사 처리.
	Fire();
}
