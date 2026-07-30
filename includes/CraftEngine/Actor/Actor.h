#pragma once

#include "Core/Core.h"
#include <memory>

namespace Craft
{
	//전방선언.
	class Level;

	class CRAFT_API Actor
	{
	public:
		Actor();
		virtual ~Actor();

	public:
		//프레임 이벤트 함수.
		void BeginPlay();
		virtual void Tick(float deltaTime);
		void Draw();

		// 액터를 레벨에서 제거할 때 사용할 함수.
		void Destroy();

		// 게임(엔진)종료 요청 함수
		void QuitGame();

		//Getter.
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		std::shared_ptr<Level> GetOwner();
		void SetOwner(std::weak_ptr<Level> newOwner);

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그.
		bool isActive = true;

		// 액터 삭제 예약 설정 플래그.
		bool hasExpired = false;

		// Owner - 액터를 소유하는 레벨 객체
		std::weak_ptr<Level> owner;
	};
}


