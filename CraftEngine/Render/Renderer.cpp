#include "Renderer.h"
#include "ScreenBuffer.h"
#include "Camera/CameraManager.h"
#include <cassert>
#include <windows.h>
#include <iostream>

namespace Craft
{

#pragma region[Frame]
	Renderer::Frame::Frame(int bufferCount)
	{
		//2차원 글자 배열 및 그리기 순서 배열 객체 생성.
		charInfoArray = std::make_unique<CHAR_INFO[]>(bufferCount);
		sortingOrderArray = std::make_unique<int[]>(bufferCount);
	}

	Renderer::Frame::~Frame()
	{

	}

	void Renderer::Frame::Clear(const Vector2Int& screenSize)
	{
		// 이중 루프를 순회하면서 값 정리.
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				//2차원 배열 인덱스(x, y)를 1차원 인덱스로 변환.
				const int index = (y * width) + x;

				//글자 값 초기화
				CHAR_INFO& info = charInfoArray[index];
				info.Char.AsciiChar = ' ';
				info.Attributes = 0;

				//그리기 순서 값 초기화
				sortingOrderArray[index] = -1;
			}
		}
	}
#pragma endregion

	//전역 변수 초기화
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2Int& screenSize)
		:screenSize(screenSize)
	{
		assert(!instance);
		instance = this;

		// 이중 버퍼에 사용할 버퍼 생성.
		screenBufferArray[0] = std::make_unique<ScreenBuffer>(screenSize);
		screenBufferArray[0]->Clear();

		screenBufferArray[1] = std::make_unique<ScreenBuffer>(screenSize);
		screenBufferArray[1]->Clear();

		//0번 콘솔 버퍼를 창에 설정.
		SetConsoleActiveScreenBuffer(screenBufferArray[0]->GetScreenBuffer());

		// 프레임 생성.
		const int bufferCount = screenSize.x * screenSize.y;
		frame = std::make_unique<Frame>(bufferCount);
		frame->Clear(screenSize);
	}

	Renderer::~Renderer()
	{
		instance = nullptr;

		//콘솔창 원래대로 복구.
		SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	void Renderer::Submit(const std::string& image, const Vector2Float& position, Color color, int sortingOrder)
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

	void Renderer::Draw(const CameraManager& cameraManager)
	{
		//화면 지우기.
		Clear();

		//프레임 그리기.
		DrawRenderQueue(cameraManager);

		//버퍼 교환(이중 버퍼).
		Present();
	}

	Renderer& Renderer::Get()
	{
		assert(instance && "instance should not be null");
		return *instance;
	}

	void Renderer::Clear()
	{
		//화면 지우는 콘솔 명령어 실행
		//system("cls");

		// 프레임 초기화
		frame->Clear(screenSize);

		// 콘솔 버퍼 초기화
		GetCurrentBuffer()->Clear();
	}

	void Renderer::DrawRenderQueue(const CameraManager& cameraManager)
	{
		//카메라 정보
		const Vector2Int viewPosition = cameraManager.GetViewPosition();
		const int viewWidth = cameraManager.GetViewWidth();
		const int viewHeight = cameraManager.GetViewHeight();
		const Vector2Int leftTopOrigin(viewWidth >> 1, viewHeight >> 1);

		// 렌더 큐 순회.
		for (const RenderCommand& command : renderQueue)
		{
			//그릴 문자열이 없으면 건너뛰기.
			if (command.image.empty())
			{
				continue;
			}

			//콘솔 좌표계에서는 x, y int형으로 표시
			const Vector2Int worldPosition = static_cast<Vector2Int>(command.position);
			const Vector2Int drawPosition = leftTopOrigin + (worldPosition - viewPosition);

			// y 위치가 뷰 화면을 벗어나면 건너뛰기.
			if (drawPosition.y < 0 || drawPosition.y >= viewHeight)
			{
				continue;
			}

			//그릴 문자열의 길이 확인
			const int length = static_cast<int>(command.image.length());

			//글자의 시작 위치.
			const int startX = drawPosition.x;

			//글자의 마지막 위치.
			const int endX = startX + length - 1;

			// x 위치가 화면을 벗어나면 건너뛰기.
			if (endX < 0 || startX >= viewWidth)
			{
				continue;
			}

			// 실제로 보여줄 글자의 시작 위치 및 마지막 위치 구하기. 
			const int visibleStart = startX < 0 ? 0 : startX;
			const int visibleEnd = endX >= viewWidth ? viewWidth - 1 : endX;

			//이미지를 배열로 순회하면서 프레임에 기록.
			for (int x = visibleStart; x <= visibleEnd; ++x)
			{
				// 문자 인덱스.
				const int sourceIndex = x - startX;

				// 문자를 기록할 2차원 배열의 인덱스.
				const int index = (drawPosition.y * screenSize.x) + x;

				//그리기 정렬 순서 비교.(깊이 버퍼)
				if (frame->sortingOrderArray[index] > command.sortingOrder)
				{
					continue;
				}

				// 2차원 배열에 글자 값 기록.
				frame->charInfoArray[index].Char.AsciiChar = command.image[sourceIndex];
				frame->charInfoArray[index].Attributes = static_cast<WORD>(command.color);

				// 그리기 정렬 순서 기록.
				frame->sortingOrderArray[index] = command.sortingOrder;
			}
		}

		// 현재 백버퍼(콘솔에서 사용하지 않는 버퍼)에 2차원 배열 기록.
		GetCurrentBuffer()->Draw(frame->charInfoArray.get());

		// 렌더 큐 비우기
		renderQueue.clear();

		// 콘솔 색상 초기화.
		SetConsoleTextAttribute(GetCurrentBuffer()->GetScreenBuffer(), static_cast<WORD>(Color::White));





		// 렌더 큐 순회.
		//for (const RenderCommand& command : renderQueue)
		//{
		//	//그릴 문자열이 없으면 건너뛰기.
		//	if (command.image.empty())
		//	{
		//		continue;
		//	}

		//	//콘솔 좌표계에서는 x, y int형으로 표시
		//	const Vector2Int drawPosition = static_cast<Vector2Int>(command.position);

		//	// y 위치가 화면을 벗어나면 건너뛰기.
		//	if (drawPosition.y < 0 || drawPosition.y >= screenSize.y)
		//	{
		//		continue;
		//	}

		//	//그릴 문자열의 길이 확인
		//	const int length = static_cast<int>(command.image.length());
		//	
		//	//글자의 시작 위치.
		//	const int startX = drawPosition.x;

		//	//글자의 마지막 위치.
		//	const int endX = startX + length - 1;

		//	// x 위치가 화면을 벗어나면 건너뛰기.
		//	if (endX < 0 || startX >= screenSize.x)
		//	{
		//		continue;
		//	}

		//	// 실제로 보여줄 글자의 시작 위치 및 마지막 위치 구하기. 
		//	const int visibleStart = startX < 0 ? 0 : startX;
		//	const int visibleEnd = endX >= screenSize.x ? screenSize.x - 1 : endX;

		//	//이미지를 배열로 순회하면서 프레임에 기록.
		//	for (int x = visibleStart; x <= visibleEnd; ++x)
		//	{
		//		// 문자 인덱스.
		//		const int sourceIndex = x - startX;
		//		
		//		// 문자를 기록할 2차원 배열의 인덱스.
		//		const int index = (drawPosition.y * screenSize.x) + x;

		//		//그리기 정렬 순서 비교.(깊이 버퍼)
		//		if (frame->sortingOrderArray[index] > command.sortingOrder)
		//		{
		//			continue;
		//		}

		//		// 2차원 배열에 글자 값 기록.
		//		frame->charInfoArray[index].Char.AsciiChar = command.image[sourceIndex];
		//		frame->charInfoArray[index].Attributes = static_cast<WORD>(command.color);

		//		// 그리기 정렬 순서 기록.
		//		frame->sortingOrderArray[index] = command.sortingOrder;
		//	}
		//}

		//// 현재 백버퍼(콘솔에서 사용하지 않는 버퍼)에 2차원 배열 기록.
		//GetCurrentBuffer()->Draw(frame->charInfoArray.get());

		//// 렌더 큐 비우기
		//renderQueue.clear();

		//// 콘솔 색상 초기화.
		//SetConsoleTextAttribute(GetCurrentBuffer()->GetScreenBuffer(), static_cast<WORD>(Color::White));
	}

	void Renderer::Present()
	{
		// 현재 백버퍼를 화면에 활성화.
		SetConsoleActiveScreenBuffer(GetCurrentBuffer()->GetScreenBuffer());

		// 버퍼 순번 교환. 1 <=> 0 교차 반복
		// One Minus X. (1-x)
		currentBufferIndex = 1 - currentBufferIndex;
	}

	const ScreenBuffer* const Renderer::GetCurrentBuffer() const
	{
		return screenBufferArray[currentBufferIndex].get();
	}
}