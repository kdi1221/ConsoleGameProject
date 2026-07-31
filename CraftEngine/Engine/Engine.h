#pragma once

#include "Core/Core.h"
#include <memory>

namespace Craft
{
	//전방선언.
	class Level;
	class Input;
	class Renderer;

	//메인 엔진 클래스
	//엔진 루프 제공
	//게임 엔진의 핵심 기능 제공
	class CRAFT_API Engine
	{
		//엔진 설정 구조체.
		struct EngineSetting
		{
			// 목표 프레임 속도.
			float framerate = 0.f;

			// 화면 가로 크기
			int width = 0;

			// 화면 세로 크기
			int height = 0;
		};

	public:
		Engine();
		virtual ~Engine();

	public:
		// 게임 루프 실행 함수
		void Run();

		// 엔진 종료 함수
		void Quit();

		// 레벨 추가 요청 함수
		template<typename T, typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		void AddNewLevel()
		{
			nextLevel = std::make_shared<T>();
		}

		// 싱글톤 접근 함수
		static Engine& Get();

		// Getter.
		inline int GetWidth() const { return setting.width; }
		inline int GetHeight() const { return setting.height; }

	protected:
		// 입력 처리 함수. (입력 폴링)
		void ProcessInput();

	// 초기화 함수.
		//레벨 초기화 함수
		void OnInitialized();

		// 게임 플레이 초기화 함수
		void BeginPlay();
		
		// 게임 플레이 업데이트 함수.
		void Tick(float deltaTime);

		// 화면에 그리는 함수.
		void Draw();

		// 이전 입력을 저장하는 함수.
		void SavePreviousInputStates();

		// 엔진 종료 시 정리 함수.
		void Shutdown();

		// 엔진 설정 로드 함수.
		void LoadEngineSetting();

	protected:
		// 엔진 설정.
		EngineSetting setting;

		// 엔진 종료 플래그.
		bool isQuit = false;

		// 싱글톤을 위한 전역 객체.
		static Engine* instance;

		// 메인 레벨.
		std::shared_ptr<Level> mainLevel;

		// 추가 요청된 레벨.
		std::shared_ptr<Level> nextLevel;

		// 입력 시스템.
		std::unique_ptr<Input> input;

		//렌더러
		std::unique_ptr<Renderer> renderer;
	};
}

