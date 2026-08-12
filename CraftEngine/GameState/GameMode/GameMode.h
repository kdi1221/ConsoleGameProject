#pragma once

#include "GameState/SessionData.h"

namespace Craft
{
	class CRAFT_API GameMode : public SessionData
	{
		TYPE_DECLARATIONS(GameMode, SessionData)

	public:
		GameMode();
		virtual ~GameMode();
	};
}


