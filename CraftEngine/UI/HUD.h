#pragma once

#include "Core/CraftObject.h"

namespace Craft
{
	/* 플레이어 HUD Base 클래스 */
	class Level;
	class CRAFT_API HUD : public CraftObject
	{
		TYPE_DECLARATIONS(HUD, CraftObject)

	public:
		HUD();
		virtual ~HUD();

	public:
		virtual void InitializeHUD(std::weak_ptr<Level> activeLevel);

	protected:
		std::shared_ptr<Level> GetLevel() const;

	private:
		std::weak_ptr<Level> level;
	};
}


