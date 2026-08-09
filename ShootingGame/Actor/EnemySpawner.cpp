#include "EnemySpawner.h"
#include "Util/Util.h"
#include "Level/Level.h"
#include "Actor/Enemy.h"

using namespace Craft;

//적 생성할 때 사용할 글자 값
static std::wstring enemyType[] =
{
	L";:^:;",
	L"zZwZz",
	L"oO@Oo",
	L"<-=->",
	L")qOp(",
	L"(o0o)"
};

EnemySpawner::EnemySpawner()
{
	//적 생성 타이머 설정
	timer.SetTargetTime(Util::RandomRange(2.f, 3.f));
}

void EnemySpawner::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//타이머 업데이트.
	timer.Tick(deltaTime);

	//경과 시간 확인
	if (!timer.IsTimeOut())
	{
		return;
	}

	//타이머 초기화
	timer.Reset();

	//적 생성
	SpawnEnemy();
}

void EnemySpawner::SpawnEnemy()
{
	//적 생성
	//적 타입의 항목 수 계산.
	const int count = sizeof(enemyType) / sizeof(enemyType[0]);

	//랜덤 인덱스.
	const int index = Util::RandomRange(0, count - 1);

	// 생성 y 위치.
	const int yPosition = Util::RandomRange(1, 10);

	// 적 생성 요청.
	GetOwner()->SpawnActor<Enemy>(enemyType[index], yPosition);
}
