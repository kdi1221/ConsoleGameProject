#pragma once

#include "Core/CraftObject.h"

namespace Craft
{
	// 전방 선언.
	class Actor;

	//Actor에 추가되어 기능을 나누는 Component 기반 클래스
	class CRAFT_API Component : public CraftObject
	{
		//RTTI 등록
		TYPE_DECLARATIONS(Component, CraftObject)

	public:
		Component();
		virtual ~Component();

	public:
		//프레임 이벤트 함수.
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		//충돌 시 호출될 충돌 이벤트 함수.
		virtual void OnCollision(const std::shared_ptr<Actor>& other);

		//Getter / Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline std::shared_ptr<Actor> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<Actor> newOwner) { owner = newOwner; }

	protected:
		// BeginPlay 이벤트 처리 여부 플래그.
		bool hasBeganPlay = false;

		// 오너십 - 이 컴포넌트를 소유한 액터
		std::weak_ptr<Actor> owner;

	};
}



