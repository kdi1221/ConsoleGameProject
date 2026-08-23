#pragma once

#include "Core/Core.h"
#include <Math/Vector2Int.h>

namespace Craft
{
	// 입력 관련 기능 제공
	class CRAFT_API Input
	{
		//Engine 클래스에서만 특정 함수에 접근 가능하도록.
		friend class Engine;

		// 키 입력 상태 구조체.
		struct KeyState
		{
			//현재 프레임에 키가 눌렸는지 여부 플래그
			bool isKeyDown = false;

			//이전 프레임에 키가 눌렸는지 여부 플래그
			bool wasKeyDown = false;
		};

	public:
		Input();
		~Input();

	public:
		// 이전 프레임에서 Unpress => 현재 프레임에서 Press 될때 1번만 호출
		bool GetKeyDown(int keyCode) const;

		// 이전 프레임에서 Press => 현재 프레임에서 Unpress 될때 1번만 호출
		bool GetKeyUp(int keyCode) const;

		// 현재 프레임에 입력이 눌리면 계속 호출
		bool GetKey(int keyCode) const;

		//현재 마우스 포인터의 콘솔 셀 좌표를 반환.
		const Vector2Int& GetMousePosition() const { return mousePosition; }

		// 싱글톤 접근 함수.
		static Input& Get();

	private:
		/* 입력 객체 초기화 */
		bool InitializeInput();

		//현재 프레임에 키 입력이 눌렸는지 여부 확인 함수.
		void ProcessInput();

		//현재 프레임의 입력 상태를 이전 프레임 상태로 저장하는 함수.
		void SavePreviousStates();

	private:
		//콘솔 이벤트 큐 방식
		//void ProcessConsoleInputEventQueue();

		//입력 이벤트 폴링 방식
		void ProcessInputPolling();

		//폴링방식 - 포커싱 여부 확인
		void UpdateCurrentFocus();

		//입력 버퍼 초기화
		void ClearInputBuffer();

	private:
		// 가상 키의 수.
		const int keyCount = 256;

		// 키 입력 상태 관리 배열.
		KeyState keyStates[256] = { 0 };

		//콘솔 입력 이벤트를 읽기 위한 핸들
		HANDLE inputHandle = INVALID_HANDLE_VALUE;

		//프로그램 시작 시 설정되어 있던 콘솔 입력 모드
		DWORD originalConsoleMode = 0;

		// 종료할 때 기존 콘솔 입력 모드를 복구할지 여부.
		bool shouldRestoreConsoleMode = false;



		//현재 콘솔 윈도우 창 핸들(포커스 관리용)
		HWND consoleWindowHandle = nullptr;

		//현재 콘솔 폰트 픽셀 크기 정보
		CONSOLE_FONT_INFO consoleFontInfo;

		//현재 창 포커스 여부
		bool bFocusWindow = false;

		//직전 프레임에서의 창 포커스 여부
		bool bWasFocusWindow = false;


		



		//현재 마우스 포인터의 콘솔 셀 좌표
		Vector2Int mousePosition = Vector2Int::Zero;

		// 싱글톤 구현을 위한 전역 변수.
		static Input* instance;
	};
}
