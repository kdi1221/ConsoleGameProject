#include "InputComponent.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Engine/Config/ConfigBase.h"
#include "Camera/CameraManager.h"

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

	void InputComponent::PreTick(float deltaTime)
	{
		super::PreTick(deltaTime);

		const Engine& engine = Engine::Get();
		const Input& input = Input::Get();
		const ConfigBase& configBase = engine.GetConfig<ConfigBase>();
		const CameraManager& cameraManager = engine.GetCameraManager();

		//마우스 위치 갱신
		const Vector2Int& currentMouseCursorPos = input.GetMousePosition();

		//카메라가 현재 보고 있는 위치(중심)
		const Vector2Int& cameraViewPos = cameraManager.GetViewPosition();

		//현재 좌상단 월드 위치
		const Vector2Int positionLeftTop(cameraViewPos.x - (configBase.GetViewWidth() >> 1),
										cameraViewPos.y - (configBase.GetViewHeight() >> 1));
		
		//커서의 월드상 위치(좌상단 월드 위치 + 마우스 커서의 위치)
		lastMouseCursorPos = positionLeftTop + currentMouseCursorPos;

		//입력에 대한 콜백 호출
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