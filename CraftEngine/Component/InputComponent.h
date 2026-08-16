#pragma once

#include "Component.h"
#include "Core/Core.h"
#include <functional>
#include <map>

namespace Craft
{
	/* 입력 카테고리 종류 */
	enum class eInputTrigger : unsigned int
	{
		None = 0,
		Down = 1 << 0,
		Up = 1 << 1,
		Press = 1 << 2,
	};

	//eInputTrigger의 비트플래그동작을 위한 연산자 오버로딩
	inline constexpr eInputTrigger operator|(eInputTrigger lhs, eInputTrigger rhs)
	{
		return static_cast<eInputTrigger>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
	}

	inline constexpr eInputTrigger operator&(eInputTrigger lhs, eInputTrigger rhs)
	{
		return static_cast<eInputTrigger>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
	}

	//플레이어 입력 컴포넌트
	class CRAFT_API InputComponent : public Component
	{
		TYPE_DECLARATIONS(InputComponent, Component)

	public:
		using InputCallback = std::function<void(int, eInputTrigger)>;

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
		virtual void PreTick(float deltaTime) override;

	public:
		void AddInputCallback(const int keyCode, const FInputTrigger& triggerInfo);

	private:
		std::map<int, FInputTrigger> mapInputCallbacks;
	};
}

