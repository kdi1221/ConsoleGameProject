#include "Actor.h"
#include <Engine/Engine.h>
#include "Level/Level.h"
#include "Render/Renderer.h"

namespace Craft
{
	Actor::Actor(const Vector2Float& position)
	{
		transform = std::make_shared<TransformComponent>(position);
		assert(transform && "transformComponent invalid..");
	
		//위치 변경시 호출되는 이벤트 대기
		transform->SetUpdatedPositionCallback(
			std::bind(&Actor::OnUpdatedPosition,
				this,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3,
				std::placeholders::_4));
	}

	//Actor::Actor(const std::string& image, const Vector2& position, Color color)
	//	:image(image), position(position), color(color), width(static_cast<int>(image.length()))
	//{
	//	//모든 Actor가 가지는 TransformComponent 생성.
	//	transform = std::make_shared<TransformComponent>(position);
	//}

	Actor::~Actor()
	{

	}

	void Actor::BeginPlay()
	{
		// 중복 호출 방지를 위해 설정.
		hasBeganPlay = true;

		// Actor의 BeginPlay 이벤트를 Component에 전달
		for (const std::shared_ptr<Component>& component : componentList)
		{
			if (!component->HasBeganPlay())
			{
				component->BeginPlay();
			}
		}
	}

	void Actor::Tick(float deltaTime)
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		// Actor의 Tick 이벤트를 Component에 전달
		for (const std::shared_ptr<Component>& component : componentList)
		{
			component->Tick(deltaTime);
		}
	}

	void Actor::Draw()
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		// 렌더러에 그릴 데이터 설정
		//Renderer::Get().Submit(image, position, color, sortingOrder);

		// Actor의 Draw 이벤트를 Component에 전달
		for (const std::shared_ptr<Component>& component : componentList)
		{
			component->Draw();
		}
	}

	void Actor::OnCollision(const std::shared_ptr<Actor>& other)
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		// Actor의 Collision 이벤트를 Component에 전달
		for (const std::shared_ptr<Component>& component : componentList)
		{
			component->OnCollision(other);
		}
	}

	//void Actor::ChangeImage(const std::string& newImage)
	//{
	//	// 새로운 문자열 복사.
	//	width = static_cast<int>(newImage.size());
	//	image = newImage;
	//}

	void Actor::Destroy()
	{
		// 액터 삭제 예약
		// 다음 프레임에 액터가 레벨에서 제거됨.
		hasExpired = true;

		// 자식 Actor가 있다면 자식 액터들도 함께 제거 처리.
		for (const std::weak_ptr<Actor>& child : children)
		{
			std::shared_ptr<Actor> childActor = child.lock();
			if (childActor)
			{
				childActor->Destroy();
			}
		}

		/* Destroy될때 이벤트 호출 */
		if (onDestroyed)
		{
			onDestroyed(weak_from_this());
		}
	}

	void Actor::QuitGame()
	{
		// 엔진 종료 요청
		Engine::Get().Quit();
	}

	void Actor::SavePreviousState()
	{
		//previousPosition = position;
		if (transform)
		{
			transform->SavePreviousWorldPosition();
		}
	}

	void Actor::AttachTo(const std::shared_ptr<Actor>& newParent, bool keepWorldPosition)
	{
		// 부모가 없거나 부모로 설정할 포인터가 this이면 건너뛰기.
		if (!newParent || newParent.get() == this)
		{
			return;
		}

		// 기존의 부모 정보가 있다면, 제거.
		DetachFromParent();

		//새로운 부모 Actor 지정
		parent = newParent;
		//newParent->children.emplace_back(shared_from_this());
		newParent->children.emplace_back(weak_from_this());

		//위치 설정
		if (transform && newParent->GetTransform())
		{
			//부모와 연결 전 월드 위치 저장.
			Vector2Float worldPosition = transform->GetWorldPosition();

			// 트랜스폼의 부모 설정.
			transform->SetParent(newParent->GetTransform());

			//월드 위치 옵션에 따른 처리
			if (keepWorldPosition)
			{
				transform->SetWorldPosition(worldPosition);
			}
		}
	}

	void Actor::DetachFromParent()
	{
		// 기존의 부모 확인.
		std::shared_ptr<Actor> oldParent = parent.lock();

		//기존의 부모가 설정된 경우에는, 기존 부모의 자식 목록에서 this를 제거
		if (oldParent)
		{
			//부모의 자손 목록 가져오기
			auto& siblingList = oldParent->children;

			for (auto iterator = siblingList.begin();
				iterator != siblingList.end();
				++iterator)
			{
				//기존 부모의 자손 목록에서 나를 찾아 제거.
				if ((*iterator).lock().get() == this)
				{
					siblingList.erase(iterator);
					break;
				}
			}
		}

		// 기존의 부모 참조 변수 초기화.
		parent.reset();

		if (transform)
		{
			//부모와의 연결 해제 후에 화면 위치 유지
			Vector2Float worldPosition = transform->GetWorldPosition();
			transform->SetParent(std::weak_ptr<TransformComponent>());
			transform->SetWorldPosition(worldPosition);
		}
	}

	void Actor::SetOwner(std::weak_ptr<Level> newOwner)
	{
		owner = newOwner;

		//Component오너십도 업데이트
		BindComponentOwners();
	}

	Vector2Float Actor::GetPosition() const
	{ 
		//트랜스폼의 위치 반환
		return transform ? transform->GetLocalPosition() : Vector2Float::Zero;
	}

	Vector2Float Actor::GetWorldPosition() const
	{
		//트랜스폼의 위치 반환
		return transform ? transform->GetWorldPosition() : Vector2Float::Zero;
	}

	void Actor::SetPosition(const Vector2Float& newPosition)
	{
		// 변경하려는 위치 값이 현재 위치와 같은지 비교.
		if (GetPosition() == newPosition)
		{
			return;
		}

		//위치 업데이트.
		if (transform)
		{
			transform->SetLocalPosition(newPosition);
		}
	}

	Vector2Float Actor::GetPreviousPosition() const
	{
		// 트랜스폼에서 관리하는 이전 프레임 위치 반환
		return transform ? transform->GetPreviousWorldPosition() : Vector2Float::Zero;
	}

	void Actor::SetUpdatedPositionCallback(const OnPositionUpdatedType& inCallback)
	{
		onPositionUpdated = inCallback;
	}

	void Actor::SetUpdatedPositionCallback(OnPositionUpdatedType&& inCallback)
	{
		onPositionUpdated = std::move(inCallback);
	}

	void Actor::SetOnDestroyedCallback(const OnDestroyCallbackType& inCallback)
	{
		onDestroyed = inCallback;
	}

	void Actor::SetOnDestroyedCallback(OnDestroyCallbackType&& inCallback)
	{
		onDestroyed = std::move(inCallback);
	}

	void Actor::ProcessAddComponents()
	{
		//추가 요청된 Component 목록이 있는지 확인.
		if (addRequestedComponentList.empty())
		{
			return;
		}

		// Component의 Owner를 현재 Actor로 설정.
		BindComponentOwners();

		// 추가 요청된 컴포넌트를 ComponentList에 옮기기.
		for (const std::shared_ptr<Component>& component : addRequestedComponentList)
		{
			//컴포넌트 null여부 확인.
			if (!component)
			{
				continue;
			}

			//실제 목록에 추가.
			componentList.emplace_back(component);

			//Actor가 이미 BeginPlay를 처리했다면 추가된 컴포넌트에서도 호출
			if (hasBeganPlay && !component->HasBeganPlay())
			{
				component->BeginPlay();
			}
		}

		//추가 처리가 완료된 목록 초기화
		addRequestedComponentList.clear();
	}

	void Actor::BindComponentOwners()
	{
		std::shared_ptr<Actor> actor = shared_from_this();
		if (!actor)
		{
			return;
		}

		// 트랜스폼 컴포넌트의 오너십 설정.
		if (transform)
		{
			transform->SetOwner(actor);
		}

		//컴포넌트 목록을 순회하면서 오너십 설정.
		for (const std::shared_ptr<Component>& component : componentList)
		{
			if (component)
			{
				component->SetOwner(actor);
			}
		}

		for (const std::shared_ptr<Component>& component : addRequestedComponentList)
		{
			if (component)
			{
				component->SetOwner(actor);
			}
		}
	}

	void Actor::OnUpdatedPosition(const Vector2Float& prevLocalPosition, 
									const Vector2Float& prevWorldPosition, 
									const Vector2Float& localPosition, 
									const Vector2Float& worldPosition)
	{
		if (onPositionUpdated)
		{
			onPositionUpdated(weak_from_this(), prevWorldPosition, worldPosition);
		}
	}
}