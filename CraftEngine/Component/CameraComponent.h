#pragma once

#include "Component.h"
#include "Math/Vector2Int.h"

namespace Craft
{
	//카메라 조작 기능 담당 컴포넌트
	class CRAFT_API CameraComponent : public Component
	{
		TYPE_DECLARATIONS(CameraComponent, Component)

	public:
		CameraComponent();
		virtual ~CameraComponent() = default;

	public:
		void SetViewPosition(const Vector2Int& viewPosition);
	};
}


