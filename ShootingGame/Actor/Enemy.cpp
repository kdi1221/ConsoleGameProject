#include "Enemy.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Engine/Config/ConfigBase.h"
#include "Level/Level.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include "DestroyEffect.h"
#include "GameManager.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"

using namespace Craft;

//콜리전 너비를 반환하는 헬퍼 함수.
namespace
{
	int GetCollisionWidth(const Actor& actor)
	{
		std::shared_ptr<BoxCollisionComponent> collision = actor.GetComponent<BoxCollisionComponent>();

		return collision ? collision->GetWidth() : 0;
	}
}

Enemy::Enemy(const std::wstring& image, int yPosition)
	:Actor(Vector2Float::Zero)
{
	//필요한 컴포넌트 추가
	AddComponent<SpriteRendererComponent>(image, Color::White, 2);
	AddComponent<BoxCollisionComponent>(static_cast<int>(image.size()));

	//랜덤(오른쪽 또는 왼쪽으로 이동할지 결정).
	int random = Util::RandomRange(1, 10);

	//랜덤으로 선택된 수가 짝수인 경우.
	if (random % 2 == 0)
	{
		//화면 오른쪽에서 생성
		direction = MoveDirection::Left;

		//x위치 설정
		xPosition = static_cast<float>(Engine::Get().GetConfig<ConfigBase>().GetDisplayWidth() - 1 - GetCollisionWidth(*this));
	}
	else
	{
		//화면 왼쪽에서 생성
		direction = MoveDirection::Right;

		//x위치 설정
		xPosition = 0.f;
	}

	//이동 방향에 따른 적 위치 설정.
	SetPosition(Vector2Float(xPosition, static_cast<float>(yPosition)));
	
	//발사 타이머 목표시간 설정.
	timer.SetTargetTime(Util::RandomRange(1.f, 3.f));
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 이동 처리.
	float dir = direction == MoveDirection::Left ? -1.f : 1.f;
	xPosition += moveSpeed * dir * deltaTime;

	// 좌표 검사.
	// 화면 왼쪽을 완전히 벗어난 경우.
	if (xPosition + GetCollisionWidth(*this) < 0)
	{
		Destroy();
		return;
	}

	//화면 오른쪽을 완전히 벗어난 경우.
	if (xPosition >= Engine::Get().GetConfig<ConfigBase>().GetDisplayWidth())
	{
		Destroy();
		return;
	}

	// 위치 설정.
	SetPosition(Vector2Float(xPosition, GetPosition().y));

	//발사 타이머 업데이트
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	//발사 처리.
	timer.Reset();

	//적 탄약 액터 생성 후 발사
	Vector2Float CurrentPosition = GetPosition();
	Vector2Float bulletPosition(CurrentPosition.x + (GetCollisionWidth(*this) / 2), CurrentPosition.y + 1.f);
	GetOwner()->SpawnActor<EnemyBullet>(bulletPosition, Util::RandomRange(10.f, 20.f));
}

void Enemy::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	// 충돌한 물체가 플레이어 탄약인지 확인.
	if (other->IsTypeOf<PlayerBullet>())
	{
		//사운드 재생.
		Engine::Get().PlayOneShot("Explosion.wav");

		//적 액터 제거
		Destroy();

		//플레이어 탄약 제거.
		other->Destroy();

		//죽음 이펙트 재생.
		if (GetOwner())
		{
			//적이 죽은 위치에 이펙트 생성.
			GetOwner()->SpawnActor<DestroyEffect>(GetPosition());

			// 점수 획득 처리.
			std::shared_ptr<GameManager> gameManager = GetOwner()->GetActorOfType<GameManager>();

			//게임 관리자에 점수 획득 알림
			if (gameManager)
			{
				gameManager->SetScore(gameManager->GetScore() + 1);
			}
		}
	}
}
