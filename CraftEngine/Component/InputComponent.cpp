#include "InputComponent.h"
#include "Core/Input.h"

namespace Craft
{
	InputComponent::FInputTrigger::FInputTrigger()
	{

	}

	InputComponent::FInputTrigger::FInputTrigger(eInputTrigger inTriggerCategory, const InputCallback& inCallback)
		:triggerCategory(inTriggerCategory)
		,triggerCallback(inCallback)
	{

	}

	InputComponent::InputComponent()
	{

	}

	void InputComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		const Input& input = Input::Get();
		for (const auto& callbackInfo : mapInputCallbacks)
		{
			const int keyCode = callbackInfo.first;
			const FInputTrigger& inputTrigger = callbackInfo.second;

			if (eInputTrigger::None != (inputTrigger.triggerCategory & eInputTrigger::Down))
			{
				if (input.GetKeyDown(keyCode))
				{
					inputTrigger.triggerCallback(keyCode, eInputTrigger::Down);
				}
			}

			if (eInputTrigger::None != (inputTrigger.triggerCategory & eInputTrigger::Up))
			{
				if (input.GetKeyUp(keyCode))
				{
					inputTrigger.triggerCallback(keyCode, eInputTrigger::Up);
				}
			}

			if (eInputTrigger::None != (inputTrigger.triggerCategory & eInputTrigger::Press))
			{
				if (input.GetKey(keyCode))
				{
					inputTrigger.triggerCallback(keyCode, eInputTrigger::Press);
				}
			}
		}
	}

	void InputComponent::AddInputCallback(const int keyCode, const FInputTrigger& triggerInfo)
	{
		const auto& existCallback = mapInputCallbacks.find(keyCode);
		if (existCallback != mapInputCallbacks.end())
		{
			return;
		}

		mapInputCallbacks.insert(std::pair<int, FInputTrigger>(keyCode, triggerInfo));
	}
}