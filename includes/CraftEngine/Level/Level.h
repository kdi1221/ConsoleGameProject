#pragma once

#include "Core/Core.h"
#include "Actor/Actor.h"
#include <vector>
#include <memory>

namespace Craft
{
	// 모든 게임 레벨의 기반 클래스.
	// 게임에 배치되는 액터를 관리.
	// 액터에 프레임 이벤트를 전달하는 기능.
	// 액터에 필요한 부가 기능 제공.
	class CRAFT_API Level : public std::enable_shared_from_this<Level>
	{
		//Engine 프렌드 선언.
		friend class Engine;

	public:
		Level();
		virtual ~Level();

	public:
		//레벨 초기화 이벤트 함수.
		virtual void OnInitialized();

		//프레임 이벤트 함수.
		void BeginPlay();
		void Tick(float deltaTime);
		void Draw();

		//액터 생성 함수 템플릿.
		template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> SpawnActor(Args... args)
		{
			// 새로운 액터 객체 생성.
			std::shared_ptr<T> newActor = std::make_shared<T>(std::forward<Args>(args)...);

			//추가 요청 목록에 추가.
			addRequestedActorList.emplace_back(newActor);

			//액터의 Owner 지정
			newActor->SetOwner(shared_from_this());

			//생성된 액터 반환.
			return newActor;
		}

		// Getter.
		inline bool HasInitialized() const { return hasInitialized; }

	protected:
		// 이전 프레임에 추가/제거 요청된 액터 처리 함수.
		void ProcessAddAndDestroyActors();

	protected:
		//레벨 초기화 여부 플래그
		bool hasInitialized = false;

		//레벨에 배치된 모든 액터.
		std::vector<std::shared_ptr<Actor>> actorList;

		//이번 프레임에 레벨에 추가 요청된 액터 리스트
		//현재 프레임을 안정적으로 처리 후 액터 추가 처리.
		std::vector<std::shared_ptr<Actor>> addRequestedActorList;
	};
}


