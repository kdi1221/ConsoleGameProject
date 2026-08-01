#include "GameLevel.h"
#include "Actor/Player.h"

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	//플레이어 액터 생성.
	SpawnActor<Player>();
}