#pragma once

#include "Component.h"
#include "Math/Vector2Int.h"
#include <memory>

namespace Craft
{
	//Actor의 위치 처리를 담당하는 컴포넌트
	class CRAFT_API TransformComponent : public Component
	{
		TYPE_DECLARATIONS(TransformComponent, Component)

	public:
		TransformComponent(const Vector2Int& localPosition = Vector2Int::Zero);
		virtual ~TransformComponent() = default;

		//이전 프레임 월드 위치를 저장하는 함수.
		void SavePreviousWorldPosition();

		// Getter/Setter.
		inline Vector2Int GetLocalPosition() const { return localPosition; }
		inline void SetLocalPosition(const Vector2Int& newPosition) { localPosition = newPosition; }

		Vector2Int GetWorldPosition() const;
		void SetWorldPosition(const Vector2Int& newPosition);

		inline Vector2Int GetPreviousWorldPosition() const { return previousWorldPosition; }

		inline std::shared_ptr<TransformComponent> GetParent() const { return parent.lock(); }
		inline void SetParent(std::weak_ptr<TransformComponent> newParent) { parent = newParent; }

	protected:
		//부모 기준 위치.
		//로컬 위치/월드 위치.
		Vector2Int localPosition;

		//이전 프레임 월드 위치.
		Vector2Int previousWorldPosition;

		// Scene Graph에서 부모가 되는 트랜스폼 컴포넌트 참조 변수.
		std::weak_ptr<TransformComponent> parent;
	};
}


