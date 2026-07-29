#pragma once

namespace Craft
{
	class Engine
	{
		//엔진 설정 구조체.
		struct EngineSetting
		{
			// 타겟 프레임 속도.
			float framerate = 120.f;
		};

	public:
		Engine();
		virtual ~Engine();

	public:
		// 게임 루프 실행 함수
		void Run();

		// 엔진 종료 함수
		void Quit();

		// 싱글톤 접근 함수
		static Engine& Get();

	protected:
		// 입력 처리 함수.
		void ProcessInput();

	// 초기화 함수.
		//레벨 초기화 함수
		void OnInitialized();

		// 액터 초기화 함수
		void BeginPlay();
		
		// 업데이트 함수.
		void Tick(float deltaTime);

		// 화면에 그리는 함수.
		void Draw();

		// 이전 입력을 저장하는 함수.
		void SavePreviousInputStates();

		// 엔진 종료 시 정리 함수.
		void Shutdown();

	protected:
		// 엔진 설정.
		EngineSetting setting;

		// 엔진 종료 플래그.
		bool isQuit = false;

		// 싱글톤을 위한 전역 객체.
		static Engine* instance;
	};
}

