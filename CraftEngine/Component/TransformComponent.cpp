#include "TransformComponent.h"

namespace Craft
{
	TransformComponent::TransformComponent(const Vector2Float& localPosition)
		:localPosition(localPosition)
		,previousWorldPosition(localPosition)
	{

	}

	void TransformComponent::SavePreviousWorldPosition()
	{
		previousWorldPosition = GetWorldPosition();
	}

	Vector2Float TransformComponent::GetWorldPosition() const
	{
		//부모 트랜스폼
		std::shared_ptr<TransformComponent> parentTransform = parent.lock();

		//부모 계층이 없다면 로컬 위치 반환
		if (!parentTransform)
		{
			return localPosition;
		}

		//부모가 있는 경우에는 부모의 월드 위치를 고려해서 위치 반환.
		return parentTransform->GetWorldPosition() + localPosition;
	}

	void TransformComponent::SetWorldPosition(const Vector2Float& newPosition)
	{
		//부모 트랜스폼
		std::shared_ptr<TransformComponent> parentTransform = parent.lock();

		// 부모가 없는 경우에는 새 위치를 로컬 위치에 설정.
		if (!parentTransform)
		{
			localPosition = newPosition;
			return;
		}

		//월드 위치에서 부모의 위치를 뺀 값을 로컬 위치에 저장.
		SetLocalPosition(newPosition - parentTransform->GetWorldPosition());
		//localPosition = newPosition - parentTransform->GetWorldPosition();
	}

	void TransformComponent::SetUpdatedPositionCallback(const OnUpdatedPosition& inCallback)
	{
		onUpdatePositionCallback = inCallback;
	}

	void TransformComponent::SetUpdatedPositionCallback(OnUpdatedPosition&& inCallback)
	{
		onUpdatePositionCallback = std::move(inCallback);
	}

	void TransformComponent::SetLocalPosition(const Vector2Float& newPosition)
	{ 
		localPosition = newPosition; 

		/* 위치 업데이트에 대한 콜백 호출 */
		if (onUpdatePositionCallback)
		{
			onUpdatePositionCallback(GetLocalPosition(), GetWorldPosition());
		}
	}
}