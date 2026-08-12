#pragma once

#include <Core/Core.h>
#include <Core/CraftObject.h>

namespace Craft
{
	class Level;

	class CRAFT_API SessionData : public CraftObject
	{
		TYPE_DECLARATIONS(SessionData, CraftObject)

	public:
		SessionData();
		virtual ~SessionData();

	public:
		void SetCurrentLevel(std::weak_ptr<Level> level);

	protected:
		// 현재 활성화된 레벨 캐스팅 반환
		template<typename T, typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		std::shared_ptr<T> GetCurrentLevel() const
		{
			return Cast<T>(currentLevel.lock());
		}

	private:
		// 현재 활성화된 레벨
		std::weak_ptr<Level> currentLevel;
	};
}


