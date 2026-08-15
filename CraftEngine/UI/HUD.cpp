#include "HUD.h"

namespace Craft
{
	HUD::HUD()
	{

	}

	HUD::~HUD()
	{

	}

	void HUD::InitializeHUD(std::weak_ptr<Level> activeLevel)
	{
		level = activeLevel;
	}

	std::shared_ptr<Level> HUD::GetLevel() const
	{
		return level.lock();
	}

}