#include "Input.h"
#include <assert.h>
#include <windows.h>

namespace Craft
{
	//전역 변수 초기화
	Input* Input::instance = nullptr;

	Input::Input()
	{
		assert(!instance);
		instance = this;
	}

	bool Input::GetKeyDown(int keyCode) const
	{
		return keyStates[keyCode].isKeyDown && !keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKeyUp(int keyCode) const
	{
		return !keyStates[keyCode].isKeyDown && keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKey(int keyCode) const
	{
		return keyStates[keyCode].isKeyDown;
	}

	Input& Input::Get()
	{
		assert(instance);
		return *instance;
	}

	void Input::ProcessInput()
	{
		for (int ix = 0; ix < keyCount; ++ix)
		{
			keyStates[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) != 0;
		}
	}

	void Input::SavePreviousStates()
	{
		for (KeyState& state : keyStates)
		{
			state.wasKeyDown = state.isKeyDown;
		}
	}
}

