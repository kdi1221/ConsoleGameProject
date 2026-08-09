#pragma once

#include "Component.h"
#include "Math/Vector2Int.h"
#include "Math/Vector2Float.h"
#include <memory>
#include <functional>

namespace Craft
{
	//Actor의 위치 처리를 담당하는 컴포넌트
	class CRAFT_API TransformComponent : public Component
	{
		TYPE_DECLARATIONS(TransformComponent, Component)

	public:
		using OnUpdatedPosition = std::function<void(const Vector2Float&, const Vector2Float&)>;

	public:
		TransformComponent(const Vector2Float& localPosition = Vector2Float::Zero);
		virtual ~TransformComponent() = default;

		//이전 프레임 월드 위치를 저장하는 함수.
		void SavePreviousWorldPosition();

		// Getter/Setter.
		inline Vector2Float GetLocalPosition() const { return localPosition; }
		void SetLocalPosition(const Vector2Float& newPosition);

		Vector2Float GetWorldPosition() const;
		void SetWorldPosition(const Vector2Float& newPosition);

		inline Vector2Float GetPreviousWorldPosition() const { return previousWorldPosition; }

		inline std::shared_ptr<TransformComponent> GetParent() const { return parent.lock(); }
		inline void SetParent(std::weak_ptr<TransformComponent> newParent) { parent = newParent; }

		/* 위치 업데이트 후 호출되는 콜백 지정 */
		void SetUpdatedPositionCallback(const OnUpdatedPosition& inCallback);
		void SetUpdatedPositionCallback(OnUpdatedPosition&& inCallback);

	protected:
		//부모 기준 위치.
		//로컬 위치/월드 위치.
		Vector2Float localPosition;

		//이전 프레임 월드 위치.
		Vector2Float previousWorldPosition;

		// Scene Graph에서 부모가 되는 트랜스폼 컴포넌트 참조 변수.
		std::weak_ptr<TransformComponent> parent;

		//위치 업데이트 후 호출되는 콜백
		OnUpdatedPosition onUpdatePositionCallback;
	};
}


