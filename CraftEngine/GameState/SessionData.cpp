#include "SessionData.h"

namespace Craft
{
	SessionData::SessionData()
	{
		
	}

	SessionData::~SessionData()
	{

	}

	void SessionData::SetCurrentLevel(std::weak_ptr<Level> level)
	{
		currentLevel = level;
	}

	void SessionData::OnInitializeLevel()
	{
	
	}

	void SessionData::OnDestroyedCurrentLevel()
	{
		currentLevel.reset();
	}
}