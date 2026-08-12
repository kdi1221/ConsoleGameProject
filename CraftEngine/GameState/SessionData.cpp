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
}