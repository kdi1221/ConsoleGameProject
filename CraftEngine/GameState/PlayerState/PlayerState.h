#pragma once

#include "Core/CraftObject.h"

namespace Craft
{
	class HUD;
	class Level;

	/* 플레이어 상태 베이스 클래스 */
	class CRAFT_API PlayerState : public CraftObject
	{
		TYPE_DECLARATIONS(PlayerState, CraftObject)

	public:
		PlayerState();
		virtual ~PlayerState();

	public:
		/* 플레이어 세션 데이터 초기화 */
		virtual void InitializeSessionData();

		/* 특정 레벨이 초기화후 활성화될때 호출 됨 */
		virtual void OnInitializeLevel(std::weak_ptr<Level> level);

		/* 현재 활성화되어 있던 레벨이 지워지기전 호출됨 */
		virtual void OnDestroyedCurrentLevel();

	protected:
		void InitializeHUD();

	protected:
		/* HUD 객체 생성 */
		virtual std::unique_ptr<HUD> CreateHUD() const;

	protected:
		// HUD 반환
		template<typename T, typename = std::enable_if_t<std::is_base_of<HUD, T>::value>>
		T* GetHUD() const
		{
			return Cast<T>(hud);
		}

	private:
		/* 현재 Level 객체 */
		std::weak_ptr<Level> activeLevel;

		/* HUD 객체 */
		std::unique_ptr<HUD> hud;
	};
}
