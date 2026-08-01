#include "Enemy.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "EnemyBullet.h"

using namespace Craft;

Enemy::Enemy(const std::string& image, int yPosition)
	:Actor(image)
{
	//랜덤(오른쪽 또는 왼쪽으로 이동할지 결정).
	int random = Util::RandomRange(1, 10);

	//랜덤으로 선택된 수가 짝수인 경우.
	if (random % 2 == 0)
	{
		//화면 오른쪽에서 생성
		direction = MoveDirection::Left;

		//x위치 설정
		xPosition = static_cast<float>(Engine::Get().GetWidth() - 1);
	}
	else
	{
		//화면 왼쪽에서 생성
		direction = MoveDirection::Right;

		//x위치 설정
		xPosition = 0.f;
	}

	//이동 방향에 따른 적 위치 설정.
	SetPosition(Vector2(static_cast<int>(xPosition), yPosition));
	
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
	if (xPosition + width < 0)
	{
		Destroy();
		return;
	}

	//화면 오른쪽을 완전히 벗어난 경우.
	if (xPosition >= Engine::Get().GetWidth())
	{
		Destroy();
		return;
	}

	// 위치 설정.
	SetPosition(Vector2(static_cast<int>(xPosition), position.y));

	//발사 타이머 업데이트
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	//발사 처리.
	timer.Reset();

	//적 탄약 액터 생성 후 발사
	Vector2 bulletPosition(position.x + (width / 2), position.y);
	GetOwner()->SpawnActor<EnemyBullet>(bulletPosition, Util::RandomRange(10.f, 20.f));
}
