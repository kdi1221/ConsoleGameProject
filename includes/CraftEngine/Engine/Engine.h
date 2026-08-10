#pragma once

#include "Core/Core.h"
#include <memory>
#include <string>
#include <cassert>

class Sound;
namespace Craft
{
	//전방선언.
	class Level;
	class Input;
	class Renderer;
	class CollisionSystem;
	class ConfigBase;
	class CameraManager;

	//메인 엔진 클래스
	//엔진 루프 제공
	//게임 엔진의 핵심 기능 제공
	class CRAFT_API Engine
	{
	private:
		//엔진 설정 파일 경로
		static const std::string configFilePath;

	public:
		Engine();
		virtual ~Engine();

	public:
		//엔진 초기화 함수
		void InitializeEngine();

		// 게임 루프 실행 함수
		void Run();

		// 엔진 종료 함수
		void Quit();

		//사운드 재생 함수(사운드 시스템 래퍼 함수)
		void PlayOneShot(const std::string& filename);
		void PlayBackgroundMusic(const std::string& filename);
		void StopBackgroundMusic();

		// 레벨 추가 요청 함수
		template<typename T, typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		void AddNewLevel()
		{
			nextLevel = std::make_shared<T>();
		}

		// 싱글톤 접근 함수
		static Engine& Get();

		// 설정 반환
		template<typename T, typename = std::enable_if_t<std::is_base_of<ConfigBase, T>::value>>
		const T& GetConfig() const
		{
			const ConfigBase* ptrConfig = config.get();
			assert(ptrConfig && "Invalid config..");

			return static_cast<const T&>(*ptrConfig);
		}

		//카메라 객체 반환
		CameraManager& GetCameraManager() const;

	protected:
		// 입력 처리 함수. (입력 폴링)
		virtual void ProcessInput();

		//레벨 초기화 함수
		virtual void OnInitialized();

		// 게임 플레이 초기화 함수
		virtual void BeginPlay();
		
		// 게임 플레이 업데이트 함수.
		virtual void Tick(float deltaTime);

		// 화면에 그리는 함수.
		virtual void Draw();

		// 충돌 처리 함수.
		virtual void ProcessColiision();

		// 이전 입력을 저장하는 함수.
		virtual void SavePreviousInputStates();

		// 엔진 종료 시 정리 함수.
		virtual void Shutdown();

		// 엔진 설정 로드 함수.
		void LoadEngineSetting();

	protected:
		virtual std::unique_ptr<ConfigBase> CreateConfig() const;

	protected:
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

		//충돌 시스템 객체.
		std::unique_ptr<CollisionSystem> collisionSystem;

		//사운드 시스템 객체
		std::unique_ptr<Sound> sound;

		//엔진 설정 객체
		std::unique_ptr<ConfigBase> config;

		//엔진 카메라 객체
		std::unique_ptr<CameraManager> cameraManager;
	};
}

