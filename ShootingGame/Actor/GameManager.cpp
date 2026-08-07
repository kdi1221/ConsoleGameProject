#include "GameManager.h"

using namespace Craft;

void GameManager::RegisterOnPlayerDead(OnPlayerDead callback)
{
	//콜백 함수 저장
	onPlayerDead = callback;
}

void GameManager::SetPlayerDead(const Vector2Int& deadPosition)
{
	//예외 처리 - 이미 플레이어가 죽은 상태라면 종료.
	if (isPlayerDead)
	{
		return;
	}

	//플레이어 죽음 상태 업데이트
	isPlayerDead = true;

	//죽은 위치 저장
	playerDeadPosition = deadPosition;

	//이벤트 발행(콜백 함수 호출)
	if (onPlayerDead)
	{
		onPlayerDead();
	}
}
