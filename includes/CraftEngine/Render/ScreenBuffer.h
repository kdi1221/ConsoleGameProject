#pragma once

#include <Math/Vector2Int.h>
#include <Windows.h>

namespace Craft
{
	// 이중 버퍼링 구현을 위한 화면 버퍼 클래스.
	class ScreenBuffer
	{
	public:
		ScreenBuffer(const Vector2Int& InScreenSize);
		~ScreenBuffer();

	public:
		// 콘솔 화면 초기화 - 화면 비우기
		void Clear() const;

		//전달된 글자 배열(2차원 배열)을 화면에 그리는 함수.
		void Draw(const CHAR_INFO* const charInfo) const;

		// Getter.
		inline HANDLE GetScreenBuffer() const { return screenBuffer; }

	private:
		//화면 버퍼 핸들(콘솔 버퍼)
		HANDLE screenBuffer = nullptr;

		// 화면의 크기
		Vector2Int screenSize;
	};
}



