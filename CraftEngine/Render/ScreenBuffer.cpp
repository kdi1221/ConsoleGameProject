#include "ScreenBuffer.h"
#include <cassert>

namespace Craft
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		:screenSize(screenSize)
	{
		//콘솔 버퍼 생성
		screenBuffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		//제대로 생성됬는지 확인
		assert(screenBuffer != INVALID_HANDLE_VALUE);

		auto SetConsoleWindowSize = [&]()
			{
				SMALL_RECT rect = {};
				rect.Top = 0;
				rect.Left = 0;
				rect.Right = static_cast<short>(screenSize.x - 1);
				rect.Bottom = static_cast<short>(screenSize.y - 1);
				BOOL result = SetConsoleWindowInfo(screenBuffer, TRUE, &rect);
				DWORD LastErrorCode = GetLastError();

				//창 크기 설정 성공 여부 확인.
				assert(result == TRUE);
			};

		auto SetConsoleWindowBufferSize = [&]()
			{
				COORD coord = { };
				coord.X = static_cast<short>(screenSize.x);
				coord.Y = static_cast<short>(screenSize.y);
				BOOL result = SetConsoleScreenBufferSize(screenBuffer, coord);

				//버퍼 설정 성공 여부 확인
				assert(result == TRUE);
			};

		//생성된 기존 화면의 버퍼 크기를 확인해서 화면크기와 버퍼 크기 설정 순서를 조정한다.
		CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
		GetConsoleScreenBufferInfo(screenBuffer, &ConsoleScreenBufferInfo);
		if ((screenSize.x > ConsoleScreenBufferInfo.dwSize.X) || (screenSize.y > ConsoleScreenBufferInfo.dwSize.Y))
		{
			//원하는 설정의 화면 크기가 원래 콘솔화면 사이즈보다 큰 경우 버퍼부터 설정 후 화면창 크기를 결정한다.
			SetConsoleWindowBufferSize();
			SetConsoleWindowSize();
		}
		else
		{
			//그렇지 않으면 화면창 크기 설정 후 버퍼 크기를 설정한다.
			SetConsoleWindowSize();
			SetConsoleWindowBufferSize();
		}

		// 화면 창 크기 설정.
		//SMALL_RECT rect = {};
		//rect.Top = 0;
		//rect.Left = 0;
		//rect.Right = static_cast<short>(screenSize.x - 1);
		//rect.Bottom = static_cast<short>(screenSize.y - 1);
		//BOOL result = SetConsoleWindowInfo(screenBuffer, TRUE, &rect);
		//DWORD LastErrorCode = GetLastError();

		////창 크기 설정 확인.
		//assert(result == TRUE);

		////화면 버퍼 크기 설정 및 예외처리.
		//COORD coord = { };
		//coord.X = static_cast<short>(screenSize.x);
		//coord.Y = static_cast<short>(screenSize.y);
		//result = SetConsoleScreenBufferSize(screenBuffer, coord);
		//assert(result == TRUE);

		// 커서 끄기(커서 깜빡임 방지).
		CONSOLE_CURSOR_INFO info;
		BOOL result = GetConsoleCursorInfo(screenBuffer, &info);
		assert(result == TRUE);
		info.bVisible = FALSE;
		result = SetConsoleCursorInfo(screenBuffer, &info);
		assert(result == TRUE);
	}

	ScreenBuffer::~ScreenBuffer()
	{
		// 버퍼 해제.
		if (screenBuffer)
		{
			CloseHandle(screenBuffer);
		}
	}

	void ScreenBuffer::Clear() const
	{
		//콘솔을 지우는 함수.
		//공백 문자를 화면 크기 전체에 한 번에 설정.

		//글자를 채울 시작 위치.
		COORD coord = { };
		coord.X = 0;
		coord.Y = 0;

		// 화면에 설정된 글자 수(출력용).
		DWORD writtenCount = 0;
		BOOL result = FillConsoleOutputCharacterA(
			screenBuffer,
			' ',
			screenSize.x * screenSize.y,
			coord,
			&writtenCount
		);

		assert(result == TRUE);
	}

	void ScreenBuffer::Draw(const CHAR_INFO* const charInfo) const
	{
		// 전달된 글자 배열을 화면에 한 번에 설정.

		// 버퍼 크기(2차원 배열의 크기).
		COORD bufferSize = {};
		bufferSize.X = static_cast<short>(screenSize.x);
		bufferSize.Y = static_cast<short>(screenSize.y);

		COORD bufferPosition = {};
		bufferPosition.X = 0;
		bufferPosition.Y = 0;

		//설정할 글자 영역
		SMALL_RECT writeRegion = {};
		writeRegion.Top = 0;
		writeRegion.Left = 0;
		writeRegion.Right = static_cast<short>(screenSize.x - 1);
		writeRegion.Bottom = static_cast<short>(screenSize.y - 1);

		BOOL result = WriteConsoleOutputA(
			screenBuffer,
			charInfo,
			bufferSize,
			bufferPosition,
			&writeRegion
		);

		//제대로 실행됐는지 여부 확인
		assert(result == TRUE);
	}
}
