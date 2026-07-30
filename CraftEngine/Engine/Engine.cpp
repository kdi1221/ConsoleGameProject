#include "Engine.h"
#include "Level/Level.h"
#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <cassert>

namespace Craft
{
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		// 엔진 객체가 생성될때 이전에 만든 엔진 객체가 없어야 함
		assert(!instance);
		instance = this;
	}

	Engine::~Engine()
	{
		instance = nullptr;
	}

	void Engine::Run()
	{
		//윈도우 제공 고해상도 타이머(하드웨어 타이머).

		//QueryPerformanceFrequency : 타이머의 해상도.
		// ex : 밀리세컨드(1/1000)해상도 = 1000.
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		//현재 시간 확인
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		// 프레임 시간 계산을 위한 변수.
		int64_t currentTime = counter.QuadPart;
		int64_t previousTime = currentTime;

		// 프레임 고정.
		const float oneFrameTime = 1.f / setting.framerate;

		while(!isQuit)
		{
			// 입력 처리.
			ProcessInput();

			// 현재 시간 확인.
			QueryPerformanceCounter(&counter);
			currentTime = counter.QuadPart;

			// 프레임 시간 계산 : (현재시간 - 이전시간) / 타이머 해상도
			const float deltaTime = static_cast<float>(currentTime - previousTime) / static_cast<float>(frequency.QuadPart);

			// 고정 프레임 처리
			if (deltaTime >= oneFrameTime)
			{
				// 레벨 초기화 이벤트 함수.
				OnInitialized();

				// 레벨의 액터 초기화 이벤트 함수.
				BeginPlay();

				//레벨의 액터 업데이트 함수.
				Tick(deltaTime);

				// 업데이트된 결과를 화면에 그리는 함수. 
				Draw();

				// 레벨 전환 처리
				if (nextLevel)
				{
					// 기존 레벨 정리
					if (mainLevel)
					{
						mainLevel.reset();
					}

					// 이전 프레임에 전환 요청된 레벨을 메인 레벨로 설정
					mainLevel = std::move(nextLevel);

					//전환 후 Next Level 정리
					nextLevel.reset();
				}

				//레벨 정리.
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();
				}

				//처리된 입력을 이전 프레임 입력으로 저장.
				SavePreviousInputStates();

				// 이전 프레임 시간 기록.
				previousTime = currentTime;
			}
		} // while(true)

		Shutdown();
	}

	void Engine::Quit()
	{
		// 종료 플래그 설정.
		isQuit = true;
	}

	Engine& Engine::Get()
	{
		// Assert, 필수 조건 확인.
		assert(instance);

		// 전역 객체 반환.
		return *instance;
	}

	void Engine::ProcessInput()
	{

	}

	void Engine::OnInitialized()
	{
		// 레벨에 이벤트 전달.
		if (!mainLevel)
		{
			return;
		}

		mainLevel->OnInitialized();
	}

	void Engine::BeginPlay()
	{
		// 레벨에 이벤트 전달.
		if (!mainLevel)
		{
			return;
		}

		mainLevel->BeginPlay();
	}

	void Engine::Tick(float deltaTime)
	{
		std::cout << "DeltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';

		// 레벨에 이벤트 전달.
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Tick(deltaTime);
	}

	void Engine::Draw()
	{
		// 레벨에 이벤트 전달.
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Draw();
	}

	void Engine::SavePreviousInputStates()
	{

	}

	void Engine::Shutdown()
	{

	}
}