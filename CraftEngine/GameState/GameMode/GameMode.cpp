#include "GameMode.h"
#include "GameState/PlayerState/PlayerState.h"
#include <cassert>

namespace Craft
{
	GameMode::GameMode()
	{
	
	}

	GameMode::~GameMode()
	{
	
	}

	void GameMode::Initlaize()
	{
		playerState = CreatePlayerState();
		assert(playerState && "Create PlayerState Fail..");

		playerState->InitializeSessionData();
	}

	void GameMode::OnInitializeLevel(std::weak_ptr<Level> level)
	{
		super::OnInitializeLevel(level);

		if (playerState)
		{
			playerState->OnInitializeLevel(level);
		}
	}

	void GameMode::OnDestroyedCurrentLevel()
	{
		super::OnDestroyedCurrentLevel();

		if (playerState)
		{
			playerState->OnDestroyedCurrentLevel();
		}
	}

	std::unique_ptr<PlayerState> GameMode::CreatePlayerState() const
	{
		return std::make_unique<PlayerState>();
	}
}