#include "Input.h"
#include <cassert>
#include <windows.h>

namespace Craft
{
	//전역 변수 초기화
	Input* Input::instance = nullptr;

	Input::Input()
	{
		assert(!instance && "instance should be null here.");
		instance = this;
	}

	Input::~Input()
	{
		//입력 모드 변경에 성공했으면 기존 콘솔 입력 모드로 복구.
		if (shouldRestoreConsoleMode)
		{
			SetConsoleMode(inputHandle, originalConsoleMode);
		}

		//전역 접근 변수 정리.
		instance = nullptr;
	}

	bool Input::InitializeInput()
	{
		//콘솔 입력 버퍼의 핸들을 가져옴
		inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		if (inputHandle == INVALID_HANDLE_VALUE || inputHandle == nullptr)
		{
			return false;
		}

		//현재 콘솔 입력 모드를 가져옴
		if (!GetConsoleMode(inputHandle, &originalConsoleMode))
		{
			return false;
		}

		// 변경할 콘솔 입력 모드 설정.
		DWORD inputMode = originalConsoleMode;

		// 마우스 이벤트 입력 활성화.
		inputMode |= ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT;

		// 빠른 편집 모드가 활성화되어 있으면 마우스 입력이
		// 콘솔의 드래그 선택 기능으로 처리되므로 비활성화.
		inputMode &= ~ENABLE_QUICK_EDIT_MODE;

		// 변경한 콘솔 입력 모드를 적용하고 성공 여부 저장.
		shouldRestoreConsoleMode = SetConsoleMode(inputHandle, inputMode) != FALSE;

		//현재 콘솔 윈도우 창 핸들을 가져옴(포커스 관리)
		consoleWindowHandle = GetConsoleWindow();
		if (!consoleWindowHandle)
		{
			return false;
		}

		//콘솔 출력 핸들을 가져옴(가로/세로 픽셀 크기 확인용)
		HANDLE consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!consoleOutputHandle)
		{
			return false;
		}

		// 현재 설정된 폰트 정보 가져오기 (가로/세로 픽셀 크기)
		GetCurrentConsoleFont(consoleOutputHandle, FALSE, &consoleFontInfo);

		//포커스 여부 갱신
		UpdateCurrentFocus();
		bWasFocusWindow = bFocusWindow;

		return true;
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
		assert(instance && "instance should not be null here.");
		return *instance;
	}

	void Input::ProcessInput()
	{
		//ProcessConsoleInputEventQueue();

		ProcessInputPolling();
	}

	void Input::SavePreviousStates()
	{
		bWasFocusWindow = bFocusWindow;

		for (KeyState& state : keyStates)
		{
			state.wasKeyDown = state.isKeyDown;
		}
	}

	//void Input::ProcessConsoleInputEventQueue()
	//{
	//	/*for (int ix = 0; ix < keyCount; ++ix)
	//	{
	//		keyStates[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) != 0;
	//	}*/

	//	//콘솔 입력 핸들이 유효하지 않으면 입력 처리 종료.
	//	if (inputHandle == INVALID_HANDLE_VALUE || inputHandle == nullptr)
	//	{
	//		return;
	//	}

	//	// 한 번에 읽어올 콘솔 입력 이벤트 배열.
	//	INPUT_RECORD records[128] = {};

	//	//콘솔 입력 버퍼에 대기 중인 이벤트 수.
	//	DWORD pendingEventCount = 0;

	//	//한 프레임 동안 입력 버퍼에 쌓인 이벤트를 모두 처리
	//	while (GetNumberOfConsoleInputEvents(inputHandle, &pendingEventCount) && pendingEventCount > 0)
	//	{
	//		//실제로 읽어온 이벤트 수를 저장할 변수.
	//		DWORD readEventCount = 0;

	//		//배열 크기를 넘지 않도록 한번에 읽을 이벤트 수 결정.
	//		const DWORD readCount = pendingEventCount < 128 ? pendingEventCount : 128;

	//		//콘솔 입력 버퍼에서 이벤트 읽기.
	//		if (!ReadConsoleInput(inputHandle, records, readCount, &readEventCount))
	//		{
	//			break;
	//		}

	//		//읽어온 입력 이벤트를 순서대로 처리.
	//		for (DWORD ix = 0; ix < readEventCount; ++ix)
	//		{
	//			//현재 처리할 입력 이벤트.
	//			const INPUT_RECORD& record = records[ix];

	//			switch (record.EventType)
	//			{
	//			case KEY_EVENT:
	//				{
	//					//키보드 이벤트 정보 가져오기
	//					const KEY_EVENT_RECORD& keyEvent = record.Event.KeyEvent;

	//					//입력된 키의 가상 키 코드 가져오기
	//					const WORD keyCode = keyEvent.wVirtualKeyCode;

	//					//관리하는 키 배열 범위 안에 있는지 확인.
	//					if (keyCode < keyCount)
	//					{
	//						//키가 눌렸는지 또는 해제됬는지 현재 상태에 저장
	//						KeyState& state = keyStates[keyCode];
	//						const bool isKeyDown = keyEvent.bKeyDown != FALSE;
	//						state.isKeyDown = isKeyDown;
	//					}
	//				}
	//				break;

	//			case MOUSE_EVENT:
	//				{
	//					//마우스 이벤트 정보 가져오기
	//					const MOUSE_EVENT_RECORD& mouseEvent = record.Event.MouseEvent;

	//					// 마우스 포인터의 콘솔 셀 좌표 저장.
	//					mousePosition.x = mouseEvent.dwMousePosition.X;
	//					mousePosition.y = mouseEvent.dwMousePosition.Y;

	//					//마우스 버튼과 가상 키 코드를 연결하기 위한 구조체.
	//					const struct MouseButton
	//					{
	//						// 키 상태 배열에서 사용할 가상 키 코드.
	//						int keyCode;

	//						// 마우스 이벤트에서 버튼 상태를 확인할 비트 값.
	//						DWORD buttonMask;
	//					} mouseButtons[] = {
	//						{ VK_LBUTTON, FROM_LEFT_1ST_BUTTON_PRESSED },
	//						{ VK_RBUTTON, RIGHTMOST_BUTTON_PRESSED },
	//						{ VK_MBUTTON, FROM_LEFT_2ND_BUTTON_PRESSED }
	//					};

	//					// 왼쪽, 오른쪽, 가운데 마우스 버튼 상태 처리.
	//					for (const MouseButton& button : mouseButtons)
	//					{
	//						// 버튼이 눌렸는지 비트 연산으로 확인한 후 키 상태에 저장.
	//						KeyState& state = keyStates[button.keyCode];
	//						const bool isKeyDown =
	//							(mouseEvent.dwButtonState & button.buttonMask) != 0;
	//						state.isKeyDown = isKeyDown;
	//					}
	//				}
	//				break;

	//			case FOCUS_EVENT:
	//				{
	//					//콘솔 창이 입력 포커스를 잃었는지 확인.
	//					if (!record.Event.FocusEvent.bSetFocus)
	//					{
	//						//포커스를 잃는 동안 KeyUp 이벤트가 누락되어 키가 계속 눌린 상태로 남는 것을 방지
	//						for (KeyState& state : keyStates)
	//						{
	//							state.isKeyDown = false;
	//						}
	//					}
	//				}
	//				break;
	//			}
	//		}
	//	}
	//}

	void Input::ProcessInputPolling()
	{
		UpdateCurrentFocus();

		if (bWasFocusWindow != bFocusWindow)
		{
			if (!bFocusWindow)
			{
				//포커스를 가지고 있다가 잃은 상태면 모든 입력을 초기화
				ClearInputBuffer();
			}
		}

		/* 포커스를 가지고 있지 않으면 입력처리 안함 */
		if (!bFocusWindow)
		{
			return;
		}

		//키 입력 처리
		for (int ix = 0; ix < keyCount; ++ix)
		{
			keyStates[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) != 0;
		}

		assert(consoleWindowHandle && "Invalid consoleWindowHandle");

		//화면 전체 기준 마우스 커서 위치 가져오기
		POINT cursorPos = {};
		GetCursorPos(&cursorPos);

		//콘솔 창 클라이언트 기준으로 변환
		ScreenToClient(consoleWindowHandle, &cursorPos);

		// 콘솔 창의 화면상 사각형 영역(RECT) 가져오기
		RECT clientRect = {};
		GetClientRect(consoleWindowHandle, &clientRect);

		/* 마우스 커서가 화면 클라이언트 영역 내에 있을때만 */
		if (cursorPos.x >= 0 && cursorPos.y >= 0 &&
			cursorPos.x < clientRect.right && cursorPos.y < clientRect.bottom)
		{
			// 픽셀 좌표를 콘솔 텍스트 칸(Cell) 좌표로 변환
			mousePosition.x = cursorPos.x / consoleFontInfo.dwFontSize.X;
			mousePosition.y = cursorPos.y / consoleFontInfo.dwFontSize.Y;
		}
		else
		{

		}
	}

	void Input::UpdateCurrentFocus()
	{
		bFocusWindow = consoleWindowHandle == GetForegroundWindow();
	}

	void Input::ClearInputBuffer()
	{
		//포커스를 잃는 동안 KeyUp 이벤트가 누락되어 키가 계속 눌린 상태로 남는 것을 방지
		for (KeyState& state : keyStates)
		{
			state.isKeyDown = false;
		}
	}
}

