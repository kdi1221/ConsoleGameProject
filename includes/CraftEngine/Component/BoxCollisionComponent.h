#pragma once

#include "Component.h"

namespace Craft
{
	//충돌에 사용할 너비(충돌 범위)를 관리하는 컴포넌트.
	class CRAFT_API BoxCollisionComponent : public Component
	{
		TYPE_DECLARATIONS(BoxCollisionComponent, Component)

	public:
		BoxCollisionComponent(int width = 0);
		virtual ~BoxCollisionComponent() = default;

		// Getter/Setter
		inline int GetWidth() const { return width; }
		inline void SetWidth(int newWidth) { width = newWidth; }

	protected:
		//충돌 검사에 사용할 너비
		int width = 0;
	};
}



