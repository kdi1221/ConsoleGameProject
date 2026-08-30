#pragma once

#include "Core/Core.h"
#include "Core/CraftObject.h"
#include <Actor/Actor.h>
#include <UI/Widget/Widget.h>
#include <vector>
#include <memory>

namespace Craft
{
	// 모든 게임 레벨의 기반 클래스.
	// 게임에 배치되는 액터를 관리.
	// 액터에 프레임 이벤트를 전달하는 기능.
	// 액터에 필요한 부가 기능 제공.
	class CRAFT_API Level : 
		public CraftObject,
		public std::enable_shared_from_this<Level>
	{
		TYPE_DECLARATIONS(Level, CraftObject)

		//public std::enable_shared_from_this<Level>
		//shared_from_this() : this 포인터를 shared_ptr로 변환
		//weak_from_this() - this 포인터를 weak_ptr로 변환
		//Engine 프렌드 선언.
		friend class Engine;

	private:
		using OnGamePauseType = std::function<void(bool)>;

	public:
		Level();
		virtual ~Level();

	public:
		//레벨 초기화 이벤트 함수.
		virtual void OnInitialized();

		//프레임 이벤트 함수.
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		//액터 생성 함수 템플릿.
		template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> SpawnActor(Args&& ...args)
		{
			// 새로운 액터 객체 생성.
			std::shared_ptr<T> newActor = std::make_shared<T>(std::forward<Args>(args)...);

			//추가 요청 목록에 추가.
			addRequestedActorList.emplace_back(newActor);

			//액터의 Owner 지정
			newActor->SetOwner(weak_from_this());

			//Actor 자체 초기화 함수 호출 
			newActor->Initialize();

			//레벨에 특정 Actor가 스폰되었음을 알림
			OnSpawnedActor(newActor);

			//생성된 액터 반환.
			return newActor;
		}

		//레벨에 표시될 위젯 생성 
		template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<Widget, T>::value>>
		std::shared_ptr<T> CreateWidget(Args&& ...args)
		{
			std::shared_ptr<T> createdWidget = std::make_shared<T>(std::forward<Args>(args)...);

			addRequestedwidgetList.emplace_back(createdWidget);

			//Widget의 Owner 지정
			createdWidget->SetOwnerLevel(weak_from_this());

			return createdWidget;
		}


		//액터 검색 함수(템플릿).
		//template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		//std::shared_ptr<T> FindActor()
		//{
		//	// 검색 - 형변환.
		//	for (const auto& actor : actorList)
		//	{
		//		// T 타입으로 형변환 시도.
		//		std::shared_ptr<T> targetActor = std::dynamic_pointer_cast<T>(actor);
		//		if (targetActor)
		//		{
		//			return targetActor;
		//		}
		//	}

		//	return nullptr;
		//}

		// 특정 타입의 액터를 검색해 반환하는 함수.
		template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> GetActorOfType()
		{
			// 레벨에 배치된 액터 목록 순회.
			for (std::shared_ptr<Actor>& actor : actorList)
			{
				//원하는 타입의 액터를 찾았으면 형변환 후 반환.
				if (actor->IsTypeOf<T>())
				{
					return std::static_pointer_cast<T>(actor);
				}
			}

			//못 찾은 경우 null반환
			return nullptr;
		}

		/* 인게임 일시정지 */
		void SetGamePause(bool bPause);

		/* 게임 일시정지 여부 변경시 호출되는 이벤트 콜백 설정 */
		void SetOnGamePause(OnGamePauseType callback);

		// Getter.
		inline bool HasInitialized() const { return hasInitialized; }
		inline bool GetGamePaused() const { return bGamePaused; }

	protected:
		// 이전 프레임에 추가/제거 요청된 액터 처리 함수.
		void ProcessAddAndDestroyActors();

		// 액터의 이전 프레임 상태 저장 함수.
		void SavePreviousActorStates();

	protected:
		/* 특정 Actor가 레벨안에 스폰됨 */
		virtual void OnSpawnedActor(std::shared_ptr<Actor> spawnedActor);

		/* 레벨안의 특정 Actor의 위치 업데이트 */
		virtual void OnPositionUpdateActorInLevel(std::weak_ptr<Actor> updatedActor, 
													const Vector2Int& prevWorldPosition,
													const Vector2Int& worldPosition);

		/* 레벨안의 특정 Actor Destroy */
		virtual void OnDestroyedActorInLevel(std::weak_ptr<Actor> destoryedActor);


	private:
		//레벨 초기화 여부 플래그
		bool hasInitialized = false;

		//게임 일시정지 여부
		bool bGamePaused = false;

		// 게임 일시정지 여부 업데이트시 호출되는 이벤트
		OnGamePauseType onGamePause = nullptr;

		//레벨에 배치된 모든 액터.
		std::vector<std::shared_ptr<Actor>> actorList;

		//이번 프레임에 레벨에 추가 요청된 액터 리스트
		//현재 프레임을 안정적으로 처리 후 액터 추가 처리.
		std::vector<std::shared_ptr<Actor>> addRequestedActorList;

		//레벨에 표시되는 위젯 리스트
		std::vector<std::shared_ptr<Widget>> widgetList;

		//현재 프레임에 레벨에 추가요청된 위젯 리스트
		std::vector<std::shared_ptr<Widget>> addRequestedwidgetList;
	};
}


