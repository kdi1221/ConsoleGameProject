#pragma once

#include "GameState/SessionData.h"

namespace Craft
{
	class CRAFT_API PlayerState : public SessionData
	{
		TYPE_DECLARATIONS(PlayerState, SessionData)

	public:
		PlayerState();
		virtual ~PlayerState();
	};
}
