#include "TransformComponent.h"

namespace Craft
{
	TransformComponent::TransformComponent(const Vector2Int& localPosition)
		:localPosition(localPosition)
		,previousWorldPosition(localPosition)
	{

	}

	void TransformComponent::SavePreviousWorldPosition()
	{
		previousWorldPosition = GetWorldPosition();
	}

	Vector2Int TransformComponent::GetWorldPosition() const
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

	void TransformComponent::SetWorldPosition(const Vector2Int& newPosition)
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
		localPosition = newPosition - parentTransform->GetWorldPosition();

	}
}