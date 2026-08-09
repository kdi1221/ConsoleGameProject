#pragma once

#include "Component.h"
#include "Core/Core.h"
#include <functional>
#include <map>

namespace Craft
{
	//플레이어 입력 컴포넌트
	class CRAFT_API InputComponent : public Component
	{
		TYPE_DECLARATIONS(InputComponent, Component)

	public:
		/* 입력 카테고리 종류 */
		enum class eInputTrigger
		{
			None = 0,
			Down,
			Press
		};

		using InputCallback = std::function<void(int)>;

		struct CRAFT_API FInputTrigger
		{
			eInputTrigger triggerCategory = eInputTrigger::None;
			InputCallback triggerCallback;

		public:
			FInputTrigger();
			FInputTrigger(eInputTrigger inTriggerCategory, const InputCallback& inCallback);
		};

	public:
		InputComponent();
		virtual ~InputComponent() = default;

	private:
		virtual void Tick(float deltaTime) override;

	public:
		void AddInputCallback(const int keyCode, const FInputTrigger& triggerInfo);

	private:
		std::map<int, FInputTrigger> mapInputCallbacks;
	};
}

