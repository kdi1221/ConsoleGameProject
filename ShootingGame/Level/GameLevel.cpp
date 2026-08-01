#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/EnemySpawner.h"

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	//플레이어 액터 생성.
	SpawnActor<Player>();

	//적 생성기 액터 생성
	SpawnActor<EnemySpawner>();
}