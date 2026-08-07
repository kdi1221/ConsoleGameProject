#include "Player.h"
#include "Engine/Engine.h"
#include "Engine/Config/ConfigBase.h"
#include "Core/Input.h"
#include "PlayerBullet.h"
#include "Level/Level.h"
#include "EnemyBullet.h"
#include "DestroyEffect.h"
#include "GameManager.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include "PlayerGun.h"
#include "PlayerEngineEffect.h"

using namespace Craft;

// 너비 계산에 사용할 편의 함수.
namespace
{
	int GetCollisionWidth(const Actor& actor)
	{
		//박스 콜리전 컴포넌트 가져오기
		std::shared_ptr<BoxCollisionComponent> collision = actor.GetComponent<BoxCollisionComponent>();

		return collision ? collision->GetWidth() : 0;
	}
}

Player::Player()
	:Actor(Vector2Float::Zero)
	,fireMode(FireMode::OneShot)
{
	//필요한 컴포넌트 추가
	AddComponent<SpriteRendererComponent>("<=A=>", Color::Green, 5);
	AddComponent<BoxCollisionComponent>(5);

	// 생성 위치 설정.
	float x = static_cast<float>((Engine::Get().GetConfig<ConfigBase>().GetDisplayWidth() / 2) - (GetCollisionWidth(*this) / 2));
	float y = static_cast<float>(Engine::Get().GetConfig<ConfigBase>().GetDisplayHeight() - 3);
	SetPosition(Vector2Float(x, y));

	// x위치 저장.
	xPosition = static_cast<float>(x);

	// 연사 타이머의 목표 시간 설정.
	fireTimer.SetTargetTime(fireInterval);
}

void Player::BeginPlay()
{
	super::BeginPlay();

	//계층으로 설정할 액터 생성 및 계층 설정.
	std::shared_ptr<Level> level = GetOwner();
	if (!level)
	{
		return;
	}

	//플레이어 기준 로컬 좌표에 왼쪽/오른쪽에 총구 생성.
	std::shared_ptr<PlayerGun> leftGun = level->SpawnActor<PlayerGun>(Vector2Float(1.f, -1.f));
	std::shared_ptr<PlayerGun> rightGun = level->SpawnActor<PlayerGun>(Vector2Float(3.f, -1.f));

	// 계층으로 연결.
	leftGun->AttachTo(shared_from_this(), false);
	rightGun->AttachTo(shared_from_this(), false);

	//gunList에 총구 액터 추가.
	gunList.emplace_back(leftGun);
	gunList.emplace_back(rightGun);

	//추진 효과 액터도 추가 및 계층 설정.
	engineEffect = level->SpawnActor<PlayerEngineEffect>(Vector2Float(1.f, 1.f));
	engineEffect->AttachTo(shared_from_this(), false);

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
		//폭발 사운드 재생.
		Engine::Get().PlayOneShot("Explosion.wav");

		// 플레이어 제거
		Destroy();

		// 적 탄약 제거
		other->Destroy();

		// 플레이어 죽음 처리
		if (GetOwner())
		{
			//플레이어가 죽은 위치에 죽음 이펙트 생성.
			//GetOwner()->SpawnActor<DestroyEffect>(GetWorldPosition());

			// 게임 관리자에 플레이어 죽음 알림.
			std::shared_ptr<GameManager> gameManager = GetOwner()->GetActorOfType<GameManager>();
			if (gameManager)
			{
				gameManager->SetPlayerDead(GetWorldPosition());
			}
		}
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
	const int collisionWidth = GetCollisionWidth(*this);
	if (xPosition + collisionWidth >= Engine::Get().GetConfig<ConfigBase>().GetDisplayWidth())
	{
		xPosition = static_cast<float>(Engine::Get().GetConfig<ConfigBase>().GetDisplayWidth() - collisionWidth);
	}

	// 위치 업데이트.
	Vector2Float newPosition = GetPosition();
	newPosition.x = xPosition;
	SetPosition(newPosition);
}

void Player::Fire()
{
	std::shared_ptr<Level> level = GetOwner();
	if (!level)
	{
		return;
	}

	//Scene Graph에서 자식 계층에 있는 총구 액터 목록을 순회하면서 총알 생성.
	for (const std::shared_ptr<PlayerGun>& gun : gunList)
	{
		if (!gun || !gun->IsActive())
		{
			continue;
		}

		//탄약 생성
		level->SpawnActor<PlayerBullet>(gun->GetFirePosition());
	}

	//사운드 재생.
	Engine::Get().PlayOneShot("Retro_Laser_Shoot.wav");
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
