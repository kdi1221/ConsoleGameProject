#include "Renderer.h"
#include <cassert>
#include <windows.h>
#include <iostream>

namespace Craft
{
	//전역 변수 초기화
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer()
	{
		assert(!instance);
		instance = this;

		// 콘솔 커서 안보이게 설정.
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 1;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}

	Renderer::~Renderer()
	{
		instance = nullptr;

		// 콘솔 커서 보이게 설정.
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 1;
		info.bVisible = TRUE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	}

	void Renderer::Submit(const std::string& image, const Vector2& position, Color color, int sortingOrder)
	{
		// 그릴 명령 객체 생성.
		RenderCommand command;
		command.image = image;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		// 렌더 큐에 명령 추가
		renderQueue.emplace_back(command);
	}

	void Renderer::Draw()
	{
		//화면 지우기.
		Clear();
		
		//프레임 그리기.
		DrawRenderQueue();

		//버퍼 교환(이중 버퍼).
		Present();
	}

	Renderer& Renderer::Get()
	{
		assert(instance);
		return *instance;
	}

	void Renderer::Clear()
	{
		//화면 지우는 콘솔 명령어 실행
		system("cls");
	}

	void Renderer::DrawRenderQueue()
	{
		// 렌더 큐 순회.
		for(const RenderCommand& command : renderQueue)
		{
			// TODO : 이중 버퍼 구현 시 그리기 순서를 비교해 그릴 지 여부 판단.

			// 콘솔 좌표 이동
			COORD coord;
			coord.X = static_cast<short>(command.position.x);
			coord.Y = static_cast<short>(command.position.y);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			
			// 색상 설정
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(command.color));

			// 그리기
			std::cout << command.image;

			//콘솔 색상 되돌리기
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(Color::White));
		}

		// 렌더 큐 비우기
		renderQueue.clear();
	}

	void Renderer::Present()
	{
		// TODO : 이중 버퍼 구현 시 사용
	}
}