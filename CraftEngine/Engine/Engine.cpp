#include "Engine.h"
#include <Level/Level.h>
#include <Core/Input.h>
#include "Render/Renderer.h"
#include "Physics/CollisionSystem.h"
#include "Util/Util.h"
#include "SoundSystem/Sound.h"
#include "Config/ConfigBase.h"
#include "Camera/CameraManager.h"
#include "Navigation/NavigationBase.h"
#include "GameState/GameMode/GameMode.h"
#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Craft
{
	Engine* Engine::instance = nullptr;

	const std::string Engine::configFilePath = "../Config/Setting.txt";

	Engine::Engine()
	{
		// 엔진 객체가 생성될때 이전에 만든 엔진 객체가 없어야 함
		assert(!instance && "instance is not null");
		instance = this;
	}

	Engine::~Engine()
	{
		instance = nullptr;
	}

	void Engine::InitializeEngine()
	{
		// 엔진 설정 로드.
		LoadEngineSetting();
		const ConfigBase& config = GetConfig<ConfigBase>();

		//랜덤 시드 설정
		Util::SetRandomSeed();

		// 입력 객체 생성.
		input = std::make_unique<Input>();

		// 렌더러 객체 생성
		renderer = std::make_unique<Renderer>(Vector2Int(config.GetDisplayWidth(), config.GetDisplayHeight()));

		//콜리전 시스템 객체 생성
		collisionSystem = std::make_unique<CollisionSystem>();

		//사운드 시스템 객체 생성.
		sound = std::make_unique<Sound>();

		//카메라 객체 생성.
		cameraManager = std::make_unique<CameraManager>(config.GetViewWidth(), config.GetViewHeight(), 
														config.GetViewSpaceMaxWidth(), config.GetViewSpaceMaxHeight());

		//네비게이션 시스템 객체 생성.
		navigationSystem = CreateNavigationSystem();
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
		const float oneFrameTime = 1.f / GetConfig<ConfigBase>().GetFrameRate();

		while (!isQuit)
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

				// 충돌 처리.
				ProcessColiision();

				// 업데이트된 결과를 화면에 그리는 함수. 
				Draw();

				//--- 이 위까지 호출이 완료되면 프레임 처리 완료됨 ---//

				// 레벨 전환 처리
				TransitionLevel();

				//레벨 정리.
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();

					//액터의 이전 상태 저장.
					mainLevel->SavePreviousActorStates();
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

	void Engine::PlayOneShot(const std::string& filename)
	{
		if (!sound)
		{
			return;
		}

		// 사운드 시스템 함수 호출
		sound->PlayOneShot(std::string("../Assets/Sound/") + filename);
	}

	void Engine::PlayBackgroundMusic(const std::string& filename)
	{
		if (!sound)
		{
			return;
		}

		// 사운드 시스템 함수 호출
		sound->PlayBackgroundMusic(std::string("../Assets/Sound/") + filename);
	}

	void Engine::StopBackgroundMusic()
	{
		if (!sound)
		{
			return;
		}

		// 사운드 시스템 함수 호출
		sound->StopBackgroundMusic();
	}

	void Engine::InitializeGameSessionData()
	{
		gameMode = CreateGameMode();
		assert(gameMode && "Create fail gameMode");
		gameMode->Initlaize();
	}

	void Engine::DestroyGameSessionData()
	{
		gameMode.reset();
	}

	Engine& Engine::Get()
	{
		// Assert, 필수 조건 확인.
		// 무조건 통화해야하는 조건이 있을때 사용
		// 디버그 모드에서만 동작.
		assert(instance && "instance is null");

		// 전역 객체 반환.
		return *instance;
	}

	CameraManager& Engine::GetCameraManager() const
	{
		assert(cameraManager && "Invalid cameraManager");
		return *cameraManager;
	}

	void Engine::ProcessInput()
	{
		assert(input && "input should not be null here.");

		if (!input)
		{
			return;
		}

		// 입력 처리 함수 호출.
		input->ProcessInput();
	}

	void Engine::OnInitialized()
	{
		// 레벨에 이벤트 전달.
		// 레벨이 이미 초기화되었으면 처리 안함.
		if (!mainLevel || mainLevel->HasInitialized())
		{
			return;
		}

		//초기화 이벤트 호출
		mainLevel->OnInitialized();

		// GameMode에 Level이 초기화되었음을 알림
		assert(gameMode && "Invalid gameMode");
		gameMode->OnInitializeLevel(mainLevel);
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
		//std::cout << "DeltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';

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
		if (mainLevel)
		{
			mainLevel->Draw();
		}

		//렌더러의 Draw 이벤트 호출
		renderer->Draw(GetCameraManager());
	}

	void Engine::ProcessColiision()
	{
		if (!mainLevel)
		{
			return;
		}

		// 레벨의 액터 목록을 충돌 시스템에 전달해 진행.
		collisionSystem->ProcessCollision(mainLevel->actorList);
	}

	void Engine::SavePreviousInputStates()
	{
		assert(input && "input should not null here");

		if (!input)
		{
			return;
		}

		input->SavePreviousStates();
	}

	void Engine::Shutdown()
	{

	}

	void Engine::LoadEngineSetting()
	{
		config = CreateConfig();

		const bool loadConfigResult = config->LoadEngineConfig(configFilePath);
		assert(loadConfigResult && "Load config fail..");

		//화면 크기 최대 제한 처리
		//HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		//COORD MaxConsoleSize = GetLargestConsoleWindowSize(StdHandle);
		//setting.width = min(setting.width, MaxConsoleSize.X);
		//setting.height = min(setting.height, MaxConsoleSize.Y);
	}

	void Engine::TransitionLevel()
	{
		if (!nextLevel)
		{
			return;
		}
		
		// 기존 레벨 정리
		if (mainLevel)
		{
			//기존 레벨이 정리되었음을 알림
			if (GameMode* currentGameMode = GetGameMode<GameMode>())
			{
				currentGameMode->OnDestroyedCurrentLevel();
			}

			mainLevel.reset();
		}

		// 이전 프레임에 전환 요청된 레벨을 메인 레벨로 설정
		mainLevel = std::move(nextLevel);

		//전환 후 Next Level 정리
		nextLevel.reset();

		//레벨 전환 후 네비게이션 시스템에 알림
		if (navigationSystem)
		{
			navigationSystem->SetCurrentLevel(mainLevel);
		}
	}
	
	std::unique_ptr<ConfigBase> Engine::CreateConfig() const
	{
		return std::make_unique<ConfigBase>();
	}

	std::unique_ptr<NavigationBase> Engine::CreateNavigationSystem() const
	{
		return std::make_unique<NavigationBase>();
	}

	std::unique_ptr<GameMode> Engine::CreateGameMode() const
	{
		return std::make_unique<GameMode>();
	}
}