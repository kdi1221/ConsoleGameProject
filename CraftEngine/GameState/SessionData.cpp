#include "SessionData.h"

namespace Craft
{
	SessionData::SessionData()
	{
		
	}

	SessionData::~SessionData()
	{

	}

	void SessionData::OnInitializeLevel(std::weak_ptr<Level> level)
	{
		currentLevel = level;
	}

	void SessionData::OnDestroyedCurrentLevel()
	{
		currentLevel.reset();
	}
}