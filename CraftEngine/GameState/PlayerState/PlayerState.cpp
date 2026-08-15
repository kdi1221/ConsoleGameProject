#include "PlayerState.h"
#include "UI/HUD.h"
#include <cassert>

namespace Craft
{
	PlayerState::PlayerState()
	{
	
	}

	PlayerState::~PlayerState()
	{
	
	}

	std::unique_ptr<HUD> PlayerState::CreateHUD() const
	{
		return std::make_unique<HUD>();
	}

	void PlayerState::InitializeSessionData()
	{
		/* 플레이어 세션 데이터 초기화 */
	}

	void PlayerState::OnInitializeLevel(std::weak_ptr<Level> level)
	{
		/* 특정 레벨이 활성화될 때 호출 */
		activeLevel = level;

		/* TODO : 원래라면 레벨에 스폰되는 플레이어 폰 및 HUD설정을 여기서 해야 함.. */
	}

	void PlayerState::OnDestroyedCurrentLevel()
	{
		/* 활성화되어있던 레벨이 삭제 전 호출 됨, 여기서 레벨에 종속되어있던 객체들 삭제처리 */
		hud.reset();
	}

	void PlayerState::InitializeHUD()
	{
		hud = CreateHUD();
		assert(hud && "Invalid hud");
		hud->InitializeHUD(activeLevel);
	}
}