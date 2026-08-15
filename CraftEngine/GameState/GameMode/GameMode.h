#pragma once

#include "GameState/SessionData.h"

namespace Craft
{
	class PlayerState;

	/* 게임 모드 베이스 클래스 */
	class CRAFT_API GameMode : public SessionData
	{
		TYPE_DECLARATIONS(GameMode, SessionData)

	public:
		GameMode();
		virtual ~GameMode();

	public:
		/* 게임모드 초기화 */
		virtual void Initlaize();

		/* 현재 활성화된 레벨이 초기화될 때 호출됨 */
		virtual void OnInitializeLevel(std::weak_ptr<Level> level) override;

		/* 현재 활성화되어 있던 레벨이 지워지기전 호출됨 */
		virtual void OnDestroyedCurrentLevel() override;

	public:
		// PlayerState 반환
		template<typename T, typename = std::enable_if_t<std::is_base_of<PlayerState, T>::value>>
		T* GetPlayerState() const
		{
			return Cast<T>(playerState);
		}

	protected:
		/* 플레이어 상태 객체 생성 */
		virtual std::unique_ptr<PlayerState> CreatePlayerState() const;

	private:
		/* 플레이어 상태 */
		std::unique_ptr<PlayerState> playerState;
	};
}


