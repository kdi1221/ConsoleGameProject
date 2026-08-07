#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/EnemySpawner.h"
#include "Actor/GameManager.h"
#include "Engine/Engine.h"
#include "Engine/Config/ConfigBase.h"
#include "Render/Renderer.h"
#include <sstream>

using namespace Craft;

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	//플레이어 액터 생성.
	SpawnActor<Player>();

	//적 생성기 액터 생성
	SpawnActor<EnemySpawner>();

	// 게임 관리자 생성 및 플레이어 죽음 이벤트에 함수 등록.
	gameManager = SpawnActor<GameManager>();
	if (gameManager)
	{
		gameManager->RegisterOnPlayerDead([this]()
			{
				PlayerDied();
			}
		);
	}
}

void GameLevel::Tick(float deltaTime)
{
	// 게임 오버 시 레벨 업데이트 중지.
	if (gameState == GameState::GameOverWait)
	{
		//타이머 시간 업데이트.
		timer.Tick(deltaTime);
		if (timer.IsTimeOut())
		{
			Engine::Get().Quit();
		}

		return;
	}

	Level::Tick(deltaTime);
}

void GameLevel::Draw()
{
	Level::Draw();

	//점수 보여주기
	ShowScore();

	// 게임 오버 상태라면 플레이어 죽음 표시.
	if (gameState == GameState::GameOverWait)
	{
		//플레이어 죽음 메시지를 렌더러에 제출.
		Renderer::Get().Submit("!Dead!", gameManager->GetPlayerDeadPosition());
	}
}

void GameLevel::ShowScore()
{
	//점수 문자열 조합. 10-> Score: 10.
	std::stringstream ss;
	ss << "Score: ";
	ss << gameManager->GetScore();

	//렌더러에 점수 문자열을 전달.
	Vector2Int drawStringPos(0, Engine::Get().GetConfig<ConfigBase>().GetDisplayHeight() - 1);
	Renderer::Get().Submit(ss.str(), static_cast<Vector2Float>(drawStringPos));
}

void GameLevel::PlayerDied()
{
	//게임 상태 변경
	gameState = GameState::GameOverWait;

	// 게임 오버 타이머 시간 설정.
	timer.SetTargetTime(gameOverWaitTime);
}