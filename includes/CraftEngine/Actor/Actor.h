#pragma once

#include "Core/Core.h"
#include "Core/CraftObject.h"
#include "Component/Component.h"
#include "Component/TransformComponent.h"
#include "Math/Vector2Int.h"
#include "Math/Color.h"
#include <memory>
#include <string>
#include <type_traits>	//std::is_base_of
#include <utility>		//std::forward
#include <vector>

namespace Craft
{
	//전방선언.
	class Level;

	//가상 공간에 배치될 모든 액터의 기본 클래스
	class CRAFT_API Actor : public CraftObject, 
							public std::enable_shared_from_this<Actor>
	{
		//Level 클래스를 friend로 선언.
		friend class Level;

		// 타입 정보 설정을 위한 매크로 추가.
		TYPE_DECLARATIONS(Actor, CraftObject)

	private:
		using OnPositionUpdatedType = std::function<void(std::weak_ptr<Actor>, const Vector2Int&, const Vector2Int&)>;
		using OnDestroyCallbackType = std::function<void(std::weak_ptr<Actor>)>;

	public:
		Actor(const Vector2Int& position = Vector2Int::Zero);
		virtual ~Actor();

	public:
		/* 생성직후 초기화 함수 */
		virtual void Initialize();

		//프레임 이벤트 함수.
		virtual void BeginPlay();

		virtual void PreTick(float deltaTime);
		virtual void Tick(float deltaTime);
		virtual void PostTick(float deltaTime);

		virtual void Draw();

		//충돌 시 호출될 충돌 이벤트 함수.
		virtual void OnCollision(const std::shared_ptr<Actor>& other);

		// 액터의 이미지(문자열) 값 변경
		//void ChangeImage(const std::string& newImage);

		// 액터를 레벨에서 제거할 때 사용할 함수.
		virtual void Destroy();

		// 게임(엔진)종료 요청 함수
		void QuitGame();

		//프레임 종료 후 현재 위치를 이전 위치로 저장하는 함수
		void SavePreviousState();

		//Scene Graph에서 부모 Actor를 지정하는 함수.
		void AttachTo(
			const std::shared_ptr<Actor>& newParent,
			bool keepWorldPosition = true
		);
		
		// 부모 Actor의 연결을 해제하는 함수.
		void DetachFromParent();

		// Actor에 Component를 추가 요청하는 함수
		template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
		std::shared_ptr<T> AddComponent(Args&& ...args)
		{
			// 추가하려는 컴포넌트가 TransformComponent인지 확인.
			static_assert(!std::is_same<T, TransformComponent>::value, "TransformComponent is created by an Actor.");

			//추가 요청 목록에 새로운 컴포넌트 생성 및 추가.
			std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>(args)...);

			//현재 Component 목록을 처리하는 중일 수 있기 때문에 추가 요청 목록에 보관.
			addRequestedComponentList.emplace_back(newComponent);

			return newComponent;
		}

		// Actor에서 특정 타입의 Component를 찾는 함수.
		template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
		std::shared_ptr<T> GetComponent() const
		{
			for (const std::shared_ptr<Component>& component : componentList)
			{
				if (component && component->IsTypeOf<T>())
				{
					return std::static_pointer_cast<T>(component);
				}
			}

			//추가 요청 목록에도 존재하는지 확인
			for (const std::shared_ptr<Component>& component : addRequestedComponentList)
			{
				if (component && component->IsTypeOf<T>())
				{
					return std::static_pointer_cast<T>(component);
				}
			}

			return nullptr;
		}

		//Getter/Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		inline std::shared_ptr<Level> GetOwner() const { return owner.lock(); }
		void SetOwner(std::weak_ptr<Level> newOwner);

		Vector2Int GetPosition() const;
		Vector2Int GetWorldPosition() const;
		void SetPosition(const Vector2Int& newPosition);

		//이전 위치 반환 함수.
		Vector2Int GetPreviousPosition() const;

		//액터의 문자열 너비 반환 함수.
		//inline int GetWidth() const { return width; }

		//TransformComponent 반환 함수.
		inline std::shared_ptr<TransformComponent> GetTransform() const { return transform; }

		//부모 Actor 반환 함수.
		inline std::shared_ptr<Actor> GetParent() const { return parent.lock(); }

		//자식 Actor 목록 반환 함수.
		inline const std::vector<std::weak_ptr<Actor>>& GetChildren() const { return children; }

	public:
		/* 이벤트에 대해 호출되는 콜백 지정 */
		void SetUpdatedPositionCallback(const OnPositionUpdatedType& inCallback);
		void SetUpdatedPositionCallback(OnPositionUpdatedType&& inCallback);

		void SetOnDestroyedCallback(const OnDestroyCallbackType& inCallback);
		void SetOnDestroyedCallback(OnDestroyCallbackType&& inCallback);

	protected:
		//추가 요청된 Component를 실제 목록에 추가 처리하는 함수.
		void ProcessAddComponents();

		//Component에 오너십 설정하는 함수.
		void BindComponentOwners();

	protected:
		/* 위치 변경시 호출되는 이벤트 */
		virtual void OnUpdatedPosition(const Vector2Int& prevLocalPosition,
										const Vector2Int& prevWorldPosition,
										const Vector2Int& localPosition,
										const Vector2Int& worldPosition);

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그.
		bool isActive = true;

		// 액터 삭제 예약 설정 플래그.
		bool hasExpired = false;

		// Owner - 액터를 소유하는 레벨 객체
		std::weak_ptr<Level> owner;

		// 모든 Actor가 기본으로 가지는 TransformComponent.
		std::shared_ptr<TransformComponent> transform;

		// Actor에 추가된 Component 목록.
		std::vector<std::shared_ptr<Component>> componentList;

		// 추가 요청된 Component 목록.
		std::vector<std::shared_ptr<Component>> addRequestedComponentList;

		// Scene Graph에서 부모 Actor.
		std::weak_ptr<Actor> parent;

		// Scene Graph에서 자식 Actor 목록
		std::vector<std::weak_ptr<Actor>> children;

		//// 화면에 그릴 글자(이미지).
		//std::string image;

		////글자 색상.
		//Color color = Color::White;

		////글자 길이
		//int width = 0;

		//// 그리기 정렬 순서.
		//int sortingOrder = 0;

		//// 액터 위치
		//Vector2 position;

		//// 이전 프레임 위치.
		//Vector2 previousPosition;

	private:
		/* 위치 업데이트 시 호출되는 콜백 이벤트 */
		OnPositionUpdatedType onPositionUpdated;

		/* Destroy될때 호출되는 콜백 이벤트 */
		OnDestroyCallbackType onDestroyed;
	};
}


